#ifndef VECPP_AP_MATH_INT_STORAGE_H_INCLUDED
#define VECPP_AP_MATH_INT_STORAGE_H_INCLUDED

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <tuple>

namespace vecpp {
namespace detail {

template <typename T>
constexpr T low_half(T v) {
  constexpr T mask_bits = (sizeof(T) * CHAR_BIT) / 2;
  auto mask = (~(T)0) >> mask_bits;
  return v & mask;
}

template <typename T>
constexpr T high_half(T v) {
  constexpr T mask_bits = (sizeof(T) * CHAR_BIT) / 2;
  return v >> mask_bits;
}

template <std::size_t bits, typename Word_t>
struct Int_storage {
  static_assert(std::is_unsigned_v<Word_t>);

  using Word = Word_t;
  static constexpr std::size_t size = bits;
  static constexpr std::size_t bits_per_word = sizeof(Word) * CHAR_BIT;
  static constexpr std::size_t last_word_bits =
      ((bits - 1) % bits_per_word) + 1;

  static constexpr std::size_t words = 1 + (bits - 1) / bits_per_word;
  static_assert(words >= 2, "Use a bitfield instead");

  constexpr Word& operator[](std::size_t id) { return data_[id]; }
  constexpr const Word& operator[](std::size_t id) const { return data_[id]; }

  static constexpr std::size_t which_word(std::size_t bit_pos) {
    return bit_pos / bits_per_word;
  }

  static constexpr Word which_bit(std::size_t bit_pos) {
    return bit_pos % bits_per_word;
  }

  static constexpr Word mask_bit(std::size_t bit_pos) {
    return Word(1) << which_bit(bit_pos);
  }

  constexpr const Word& get_word(std::size_t bit_pos) const {
    return data_[which_word(bit_pos)];
  }

  constexpr Word& get_word(std::size_t bit_pos) {
    return data_[which_word(bit_pos)];
  }

  constexpr bool get_bit(std::size_t bit_pos) const {
    assert(bit_pos < bits && "Bit position out of bounds!");
    return (mask_bit(bit_pos) & get_word(bit_pos)) != 0;
  }

  constexpr void set_bit(std::size_t bit_pos) {
    assert(bit_pos < bits && "Bit position out of bounds!");
    get_word(bit_pos) |= mask_bit(bit_pos);
  }

  constexpr void clear_unused_bits();
  constexpr void fill_unused_bits();
  constexpr std::size_t count_leading_zeros() const;

  constexpr void invert();
  constexpr bool add(const Int_storage& rhs);
  constexpr bool subtract(const Int_storage& rhs);
  constexpr void binary_and(const Int_storage& rhs);
  constexpr void binary_or(const Int_storage& rhs);
  constexpr void binary_xor(const Int_storage& rhs);
  constexpr void lshift(uint64_t rhs);
  constexpr void rshift(uint64_t rhs);

  constexpr int compare(const Int_storage& rhs) const;
  constexpr Word mul(Word rhs);

  constexpr std::tuple<Int_storage, Int_storage> udivmod(
      const Int_storage&) const;

  std::array<Word, words> data_;
};

template <std::size_t bits, typename Word_t>
constexpr void Int_storage<bits, Word_t>::clear_unused_bits() {
  constexpr Word word_max = ~Word(0);
  constexpr Word mask = word_max >> (bits_per_word - last_word_bits);

  data_.back() &= mask;
}

template <std::size_t bits, typename Word_t>
constexpr void Int_storage<bits, Word_t>::fill_unused_bits() {
  constexpr Word word_max = ~Word(0);
  constexpr Word mask = word_max >> (bits_per_word - last_word_bits);

  data_.back() |= ~mask;
}

template <std::size_t bits, typename Word_t>
constexpr void Int_storage<bits, Word_t>::invert() {
  for (auto& w : data_) {
    w = ~w;
  }

  clear_unused_bits();
}

// Addition is identical for signed and unsigned.
template <std::size_t bits, typename Word_t>
constexpr bool Int_storage<bits, Word_t>::add(const Int_storage& rhs) {
  bool carry = false;
  for (std::size_t i = 0; i < words; ++i) {
    auto l = data_[i];
    if (carry) {
      data_[i] += rhs[i] + 1;
      carry = data_[i] <= l;
    } else {
      data_[i] += rhs[i];
      carry = data_[i] < l;
    }
  }
  clear_unused_bits();
  return carry;
}

// Subtraction is identical for signed and unsigned.
template <std::size_t bits, typename Word_t>
constexpr bool Int_storage<bits, Word_t>::subtract(const Int_storage& rhs) {
  bool carry = false;
  for (std::size_t i = 0; i < words; ++i) {
    auto l = data_[i];
    if (carry) {
      data_[i] -= rhs.data_[i] + 1;
      carry = data_[i] >= l;
    } else {
      data_[i] -= rhs.data_[i];
      carry = data_[i] > l;
    }
  }
  return carry;
}

template <std::size_t bits, typename Word_t>
constexpr void Int_storage<bits, Word_t>::binary_and(const Int_storage& rhs) {
  for (std::size_t i = 0; i < words; ++i) {
    data_[i] &= rhs.data_[i];
  }
}
template <std::size_t bits, typename Word_t>
constexpr void Int_storage<bits, Word_t>::binary_or(const Int_storage& rhs) {
  for (std::size_t i = 0; i < words; ++i) {
    data_[i] |= rhs.data_[i];
  }
}
template <std::size_t bits, typename Word_t>
constexpr void Int_storage<bits, Word_t>::binary_xor(const Int_storage& rhs) {
  for (std::size_t i = 0; i < words; ++i) {
    data_[i] ^= rhs.data_[i];
  }
}

template <std::size_t bits, typename Word_t>
constexpr void Int_storage<bits, Word_t>::lshift(uint64_t rhs) {
  if (rhs == 0) {
    return;
  }
  std::size_t word_shift = std::min(rhs / bits_per_word, words);
  std::size_t bit_shift = rhs % bits_per_word;

  auto w = words;
  while (w-- > word_shift) {
    data_[w] = data_[w - word_shift] << bit_shift;
    if (w > word_shift) {
      data_[w] |= data_[w - word_shift - 1] >> (bits_per_word - bit_shift);
    }
  }
  for (std::size_t i = 0; i < word_shift; ++i) {
    data_[i] = 0;
  }
}

template <std::size_t bits, typename Word_t>
constexpr void Int_storage<bits, Word_t>::rshift(uint64_t rhs) {
  bool filling_ones = get_bit(bits - 1);

  if (filling_ones) {
    fill_unused_bits();
  }

  std::size_t word_shift = std::min(rhs / bits_per_word, words);
  std::size_t bit_shift = rhs % bits_per_word;

  for (std::size_t w = 0; w < (words - word_shift); ++w) {
    data_[w] = data_[w + word_shift] >> bit_shift;
    if (w + word_shift + 1 < words) {
      data_[w] |= data_[w + word_shift + 1] << (bits_per_word - bit_shift);
    }
  }

  if (filling_ones) {
    for (std::size_t w = words - word_shift; w < words; ++w) {
      data_[w] = ~Word(0);
    }

    Word mask = ~((1 << (bits_per_word - bit_shift)) - 1);
    data_[words - word_shift - 1] |= mask;
    clear_unused_bits();
  }
}

template <std::size_t bits, typename Word_t>
constexpr int Int_storage<bits, Word_t>::compare(const Int_storage& rhs) const {
  std::size_t parts = words;
  while (parts) {
    parts--;
    if (data_[parts] != rhs[parts]) {
      return (data_[parts] > rhs[parts]) ? 1 : -1;
    }
  }

  return 0;
}

template <std::size_t bits, typename Word_t>
constexpr Word_t Int_storage<bits, Word_t>::mul(Word rhs) {
  Word carry = 0;
  for (auto& v : data_) {
    Word low = 0, mid = 0, high = 0;
    auto src_part = v;
    // [ LOW, HIGH ] = MULTIPLIER * SRC[i] + DST[i] + CARRY.

    if (src_part == 0 || rhs == 0) {
      low = carry;
      high = 0;
    } else {
      low = low_half(src_part) * low_half(rhs);
      high = high_half(src_part) * high_half(rhs);

      mid = low_half(src_part) * high_half(rhs);
      high += high_half(mid);
      mid <<= bits_per_word / 2;
      if (low + mid < low) {
        ++high;
      }
      low += mid;

      mid = high_half(src_part) * low_half(rhs);
      high += high_half(mid);
      mid <<= bits_per_word / 2;
      if (low + mid < low) {
        ++high;
      }
      low += mid;

      if (low + carry < low) {
        high++;
      }
      low += carry;
    }
    v = low;
    carry = high;
  }

  clear_unused_bits();
  return carry;
}

template <std::size_t bits, typename Word_t>
constexpr std::size_t Int_storage<bits, Word_t>::count_leading_zeros() const {
  std::size_t result = 0;

  if (data_.back() != 0) {
    constexpr Word mask = Word(1) << (last_word_bits - 1);
    auto w = data_.back();
    while (1) {
      if ((w & mask) != 0) {
        break;
      }
      ++result;
      w <<= 1;
    }
  } else {
    result += last_word_bits;

    for (int i = (int)words - 2; i >= 0; --i) {
      if (data_[i] == 0) {
        result += bits_per_word;
      } else {
        constexpr Word mask = Word(1) << (bits_per_word - 1);
        auto w = data_[i];
        while (1) {
          if ((w & mask) != 0) {
            break;
          }
          ++result;
          w <<= 1;
        }
        break;
      }
    }
  }
  return result;
}

template <std::size_t bits, typename Word_t>
constexpr std::tuple<Int_storage<bits, Word_t>, Int_storage<bits, Word_t>>
Int_storage<bits, Word_t>::udivmod(const Int_storage& denum) const {
  // Good old long division
  auto cmp = compare(denum);
  if (cmp < 0) {
    return std::make_tuple(Int_storage<bits, Word_t>{0}, *this);
  }
  if (cmp == 0) {
    return std::make_tuple(Int_storage<bits, Word_t>{1},
                           Int_storage<bits, Word_t>{0});
  }

  auto divisor = denum;

  auto num_leadind_z = count_leading_zeros();
  auto den_leadind_z = denum.count_leading_zeros();
  std::int64_t shift = std::int64_t(den_leadind_z - num_leadind_z);

  Int_storage result{0};
  auto remainder = *this;

  divisor.lshift(Word(shift));

  while (shift >= 0) {
    if (remainder.compare(denum) < 0) {
      break;
    }
    auto comp = divisor.compare(remainder);
    if (comp <= 0) {
      remainder.subtract(divisor);
      result.set_bit(shift);
      if (comp == 0) {
        break;
      }
    }
    --shift;
    divisor.rshift(1);
  }

  return std::make_tuple(result, remainder);
}
}
}

#endif