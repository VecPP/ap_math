//  Copyright 2018 Francois Chabot
//  (francois.chabot.dev@gmail.com)
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef VECPP_AP_INT_COMPOSED_INCLUDED_H
#define VECPP_AP_INT_COMPOSED_INCLUDED_H

#include <algorithm>
#include <array>
#include <cassert>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>

namespace vecpp {
// This is really simple. An AP int
template <std::size_t bits>
struct Composed_ap_int_impl {
  static_assert(bits > 64);

  using Word_type = std::uint64_t;
  using Operand_type = std::int64_t;

  static constexpr std::size_t word_bits = sizeof(Word_type) * CHAR_BIT;
  static constexpr std::size_t word_count = 1 + (bits - 1) / word_bits;
  static constexpr std::size_t bits_in_last_word = bits % word_bits;
  static_assert(word_count >= 2);

  constexpr Composed_ap_int_impl() : v_{0} {};
  constexpr explicit Composed_ap_int_impl(std::int64_t v);
  constexpr Composed_ap_int_impl(const Composed_ap_int_impl&) = default;
  constexpr Composed_ap_int_impl& operator=(const Composed_ap_int_impl&) =
      default;

  static constexpr std::size_t which_word(std::size_t bit_pos) {
    return bit_pos / word_bits;
  }

  static constexpr Word_type which_bit(std::size_t bit_pos) {
    return bit_pos % word_bits;
  }

  static constexpr Word_type mask_bit(std::size_t bit_pos) {
    return Word_type(1) << which_bit(bit_pos);
  }

  constexpr Word_type get_word(std::size_t bit_pos) const {
    return v_[which_word(bit_pos)];
  }

  constexpr bool get_bit(std::size_t bit_pos) const {
    assert(bit_pos < bits && "Bit position out of bounds!");
    return (mask_bit(bit_pos) & get_word(bit_pos)) != 0;
  }

  constexpr std::size_t count_leading_zeros() const {
    std::size_t result = 0;

    // special handling of the last word
    if (v_.back() != 0) {
      //...
    } else {
      result += bits_in_last_word;

      for (int i = word_count - 2; i >= 0; --i) {
        std::cout << i << " " << v_[i] << "\n";
        if (v_[i] == 0) {
          result += word_bits;
          std::cout << "aaa\n";
        } else {
          constexpr Word_type mask = Word_type(1) << (word_bits - 1);
          Word_type w = v_[i];
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

  constexpr bool is_negative() const { return get_bit(bits - 1); }

  constexpr Composed_ap_int_impl& clear_unused_bits() {
    constexpr std::size_t last_word_bits = ((bits - 1) % word_bits) + 1;
    constexpr Word_type word_max = ~Word_type(0);
    constexpr Word_type mask = word_max >> (word_bits - last_word_bits);

    v_.back() &= mask;
    return *this;
  }

  std::array<Word_type, word_count> v_;
};

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits>::Composed_ap_int_impl(std::int64_t v) {
  using Word_type = typename Composed_ap_int_impl<bits>::Word_type;
  v_[0] = v;

  if (v >= 0) {
    for (std::size_t i = 1; i < word_count; ++i) {
      v_[i] = 0;
    }
  } else {
    for (std::size_t i = 1; i < word_count; ++i) {
      v_[i] = ~Word_type(0);
    }
    clear_unused_bits();
  }
}

// Comparisons
template <std::size_t bits>
constexpr bool operator==(const Composed_ap_int_impl<bits>& lhs,
                          const Composed_ap_int_impl<bits>& rhs) {
  return lhs.v_ == rhs.v_;
}

template <std::size_t bits>
constexpr bool operator!=(const Composed_ap_int_impl<bits>& lhs,
                          const Composed_ap_int_impl<bits>& rhs) {
  return lhs.v_ != rhs.v_;
}

template <std::size_t bits>
constexpr bool operator<(const Composed_ap_int_impl<bits>& lhs,
                         const Composed_ap_int_impl<bits>& rhs) {
  bool l_neg = lhs.is_negative();
  bool r_neg = lhs.is_negative();
  if( l_neg && !r_neg) {
    return true;
  }

  if( !l_neg && r_neg) {
    return false;
  }

  for (std::size_t i = lhs.word_count; i > 0; --i) {
    if (lhs.v_[i - 1] < rhs.v_[i - 1]) {
      return true;
    }
  }

  return false;
}

template <std::size_t bits>
constexpr bool operator<=(const Composed_ap_int_impl<bits>& lhs,
                          const Composed_ap_int_impl<bits>& rhs) {
  assert(false);
}

template <std::size_t bits>
constexpr bool operator>(const Composed_ap_int_impl<bits>& lhs,
                         const Composed_ap_int_impl<bits>& rhs) {
  for (std::size_t i = lhs.word_count; i > 0; --i) {
    if (lhs.v_[i - 1] > rhs.v_[i - 1]) {
      return true;
    }
  }

  return false;
}

template <std::size_t bits>
constexpr bool operator>=(const Composed_ap_int_impl<bits>& lhs,
                          const Composed_ap_int_impl<bits>& rhs) {
  assert(false);
}
// UNARY OPERATORS

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits> operator+(
    const Composed_ap_int_impl<bits>& rhs) {
  return rhs;
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits> operator-(
    const Composed_ap_int_impl<bits>& rhs) {
  Composed_ap_int_impl<bits> result = ~rhs;
  ++result;
  return result;
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits> operator~(
    const Composed_ap_int_impl<bits>& rhs) {
  Composed_ap_int_impl<bits> result;

  for (std::size_t i = 0; i < rhs.word_count; ++i) {
    result.v_[i] = ~rhs.v_[i];
  }
  return result.clear_unused_bits();
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits>& operator++(
    Composed_ap_int_impl<bits>& rhs) {
  using Word_type = typename Composed_ap_int_impl<bits>::Word_type;

  rhs += Word_type(1);

  return rhs;
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits> operator++(Composed_ap_int_impl<bits>& rhs,
                                                int) {
  auto tmp = rhs;
  ++rhs;
  return tmp;
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits>& operator--(
    Composed_ap_int_impl<bits>& rhs) {
  using Word_type = typename Composed_ap_int_impl<bits>::Word_type;

  rhs -= Word_type(1);

  return rhs;
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits> operator--(Composed_ap_int_impl<bits>& rhs,
                                                int) {
  auto tmp = rhs;
  --rhs;
  return tmp;
}

// binary assignments
template <std::size_t bits>
constexpr Composed_ap_int_impl<bits>& operator+=(
    Composed_ap_int_impl<bits>& lhs, const Composed_ap_int_impl<bits>& rhs) {
  assert(false);
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits> operator+(
    const Composed_ap_int_impl<bits>& lhs,
    const Composed_ap_int_impl<bits>& rhs) {
  auto tmp = lhs;
  return tmp += rhs;
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits>& operator-=(
    Composed_ap_int_impl<bits>& lhs, const Composed_ap_int_impl<bits>& rhs) {
  assert(false);
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits> operator-(
    const Composed_ap_int_impl<bits>& lhs,
    const Composed_ap_int_impl<bits>& rhs) {
  auto tmp = lhs;
  return tmp -= rhs;
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits>& operator*=(
    Composed_ap_int_impl<bits>& lhs, const Composed_ap_int_impl<bits>& rhs) {
  assert(false);
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits> operator*(
    const Composed_ap_int_impl<bits>& lhs,
    const Composed_ap_int_impl<bits>& rhs) {
  auto tmp = lhs;
  return tmp *= rhs;
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits>& operator/=(
    Composed_ap_int_impl<bits>& lhs, const Composed_ap_int_impl<bits>& rhs) {
  assert(false);
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits> operator/(
    const Composed_ap_int_impl<bits>& lhs,
    const Composed_ap_int_impl<bits>& rhs) {
  auto tmp = lhs;
  return tmp /= rhs;
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits>& operator%=(
    Composed_ap_int_impl<bits>& lhs, const Composed_ap_int_impl<bits>& rhs) {
  assert(false);
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits> operator%(
    const Composed_ap_int_impl<bits>& lhs,
    const Composed_ap_int_impl<bits>& rhs) {
  auto tmp = lhs;
  return tmp %= rhs;
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits>& operator&=(
    Composed_ap_int_impl<bits>& lhs, const Composed_ap_int_impl<bits>& rhs) {
  for (std::size_t i = 0; i < lhs.word_count; ++i) {
    lhs.v_[i] &= rhs.v_[i];
  }
  return lhs;
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits> operator&(
    const Composed_ap_int_impl<bits>& lhs,
    const Composed_ap_int_impl<bits>& rhs) {
  auto tmp = lhs;
  return tmp &= rhs;
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits>& operator|=(
    Composed_ap_int_impl<bits>& lhs, const Composed_ap_int_impl<bits>& rhs) {
  for (std::size_t i = 0; i < lhs.word_count; ++i) {
    lhs.v_[i] |= rhs.v_[i];
  }
  return lhs;
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits> operator|(
    const Composed_ap_int_impl<bits>& lhs,
    const Composed_ap_int_impl<bits>& rhs) {
  auto tmp = lhs;
  return tmp |= rhs;
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits>& operator^=(
    Composed_ap_int_impl<bits>& lhs, const Composed_ap_int_impl<bits>& rhs) {
  for (std::size_t i = 0; i < lhs.word_count; ++i) {
    lhs.v_[i] ^= rhs.v_[i];
  }
  return lhs;
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits> operator^(
    const Composed_ap_int_impl<bits>& lhs,
    const Composed_ap_int_impl<bits>& rhs) {
  auto tmp = lhs;
  return tmp ^= rhs;
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits>& operator<<=(
    Composed_ap_int_impl<bits>& lhs, const Composed_ap_int_impl<bits>& rhs) {
  assert(false);
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits> operator<<(
    const Composed_ap_int_impl<bits>& lhs,
    const Composed_ap_int_impl<bits>& rhs) {
  auto tmp = lhs;
  return tmp <<= rhs;
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits>& operator>>=(
    Composed_ap_int_impl<bits>& lhs, const Composed_ap_int_impl<bits>& rhs) {
  assert(false);
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits> operator>>(
    const Composed_ap_int_impl<bits>& lhs,
    const Composed_ap_int_impl<bits>& rhs) {
  auto tmp = lhs;
  return tmp >>= rhs;
}

// Binary composed op word
template <std::size_t bits>
constexpr Composed_ap_int_impl<bits>& operator+=(
    Composed_ap_int_impl<bits>& lhs,
    typename Composed_ap_int_impl<bits>::Word_type rhs) {
  for (auto& v : lhs.v_) {
    v += rhs;
    if (v >= rhs) {
      break;
    }
    rhs = 1;
  }
  return lhs;
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits>& operator-=(
    Composed_ap_int_impl<bits>& lhs,
    typename Composed_ap_int_impl<bits>::Word_type rhs) {
  for (auto& v : lhs.v_) {
    auto tmp = v;
    v -= rhs;
    if (rhs <= tmp) {
      break;
    }
    rhs = 1;
  }
  return lhs;
}

/*

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits> operator+(
    const Composed_ap_int_impl<bits>& lhs,
    const Composed_ap_int_impl<bits>& rhs) {
  Composed_ap_int_impl<bits> result = lhs;
  return result += rhs;
}


template <std::size_t bits>
constexpr Composed_ap_int_impl<bits>& operator-=(
    Composed_ap_int_impl<bits>& lhs,
    typename Composed_ap_int_impl<bits>& rhs) {
  for (auto& v : lhs.v_) {
    v += rhs;
    if (v >= rhs) {
      break;
    }
    rhs = 1;
  }
  return lhs;
}

template <std::size_t bits>
constexpr typename Composed_ap_int_impl<bits>::Word_type operator%(
    const Composed_ap_int_impl<bits>& lhs,
    typename Composed_ap_int_impl<bits>::Word_type rhs) {
  Composed_ap_int_impl<bits> tmp = lhs;

  // TODO: AHHHHHHHHHHHHHHHHHHHHHHHH
  while (tmp >= rhs) {
    tmp -= rhs;
  }

  return tmp.v_[0];
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits>& operator+=(
    Composed_ap_int_impl<bits>& lhs, const Composed_ap_int_impl<bits>& rhs) {
  using Word_type = typename Composed_ap_int_impl<bits>::Word_type;

  Word_type carry = 0;
  for (std::size_t i = 0; i < lhs.word_count; ++i) {
    Word_type prev = lhs.v_[i];
    if (carry) {
      lhs.v_[i] += rhs.v_[i] + 1;
      carry = (lhs.v_[i] <= prev);
    } else {
      lhs.v_[i] += rhs.v_[i];
      carry = (lhs.v_[i] < prev);
    }
  }

  return lhs.clear_unused_bits();
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits>& operator-=(
    Composed_ap_int_impl<bits>& lhs,
    typename Composed_ap_int_impl<bits>::Word_type rhs) {
  for (auto& v : lhs.v_) {
    auto tmp = v;
    v -= rhs;
    if (rhs <= v) {
      break;
    }
    rhs = 1;
  }
  return lhs;
}

template <std::size_t bits>
constexpr bool operator<(const Composed_ap_int_impl<bits>& lhs,
                         const Composed_ap_int_impl<bits>& rhs) {
  for (std::size_t i = lhs.word_count; i > 0; --i) {
    if (lhs.v_[i - 1] < rhs.v_[i - 1]) {
      return true;
    }
  }

  return false;
}

template <std::size_t bits>
constexpr bool operator>(const Composed_ap_int_impl<bits>& lhs,
                         const Composed_ap_int_impl<bits>& rhs) {
  for (std::size_t i = lhs.word_count; i > 0; --i) {
    if (lhs.v_[i - 1] > rhs.v_[i - 1]) {
      return true;
    }
  }

  return false;
}

template <std::size_t bits>
constexpr bool operator>(const Composed_ap_int_impl<bits>& lhs,
                         typename Composed_ap_int_impl<bits>::Word_type rhs) {
  if (lhs.is_negative()) {
    return false;
  }

  if (lhs.v_[0] > rhs) {
    return true;
  }

  for (std::size_t i = 1; i < lhs.word_count; ++i) {
    if (lhs.v_[i] != 0) {
      return true;
    }
  }

  return false;
}

template <std::size_t bits>
constexpr bool operator>=(const Composed_ap_int_impl<bits>& lhs,
                          typename Composed_ap_int_impl<bits>::Word_type rhs) {
  if (lhs.is_negative()) {
    return false;
  }

  if (lhs.v_[0] >= rhs) {
    return true;
  }

  for (std::size_t i = 1; i < lhs.word_count; ++i) {
    if (lhs.v_[i] != 0) {
      return true;
    }
  }

  return false;
}

template <std::size_t bits>
constexpr std::tuple<Composed_ap_int_impl<bits>, Composed_ap_int_impl<bits>>
udivmod(const Composed_ap_int_impl<bits>& num,
        const Composed_ap_int_impl<bits>& denum) {

  if(denum > num) {
    return std::make_tuple(Composed_ap_int_impl<bits>{0}, num);
  }
  auto divisor = denum;

  auto num_leadind_z = num.count_leading_zeros();
  auto den_leadind_z = denum.count_leading_zeros();

  auto shift = num_leadind_z - den_leadind_z;
  divisor <<= shift;

  Composed_ap_int_impl<bits> result{0};
  auto remainder = num;

  while (1) {
    if (remainder < denum) {
      break;
    }

    if (divisor < remainder) {
      remainder -= divisor;
      result.set_bit(shift);
    }

    --shift;
    divisor <<= 1;
  }

  return std::make_tuple(result, remainder);
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits>& operator/=(
    Composed_ap_int_impl<bits>& lhs,
    typename Composed_ap_int_impl<bits>::Word_type rhs) {
  return lhs;
}

// Status: Working well.
template <std::size_t bits>
constexpr Composed_ap_int_impl<bits>& operator<<=(
    Composed_ap_int_impl<bits>& lhs, std::size_t rhs) {
  std::size_t word_shift = std::min(rhs / lhs.word_bits, lhs.word_count);
  std::size_t bit_shift = rhs % lhs.word_bits;

  auto words = lhs.word_count;

  while (words-- > word_shift) {
    lhs.v_[words] = lhs.v_[words - word_shift] << bit_shift;
    if (words > word_shift) {
      lhs.v_[words] |=
          lhs.v_[words - word_shift - 1] >> (lhs.word_bits - bit_shift);
    }
  }

  for (std::size_t i = 0; i < word_shift; ++i) {
    lhs.v_[i] = 0;
  }

  return lhs;
}

template <std::size_t bits>
std::ostream& operator<<(std::ostream& stream,
                         const Composed_ap_int_impl<bits>& val) {
  if (val == 0) {
    return stream << "0";
  }

  if (val.is_negative()) {
    return stream << "-" << -val;
  }

  std::ostringstream result;
  auto tmp = val;
  while (tmp != 0) {
    auto offset = int(tmp % 10);
    result << char('0' + int(tmp % 10));
    tmp /= 10;
  }

  std::string result_string = result.str();
  std::reverse(result_string.begin(), result_string.end());
  stream << result_string;
  return stream;
}
*/
}

#endif