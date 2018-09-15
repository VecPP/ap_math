//  Copyright 2018 Francois Chabot
//  (francois.chabot.dev@gmail.com)
//
//  See the accompanying LICENSE file for licensing details.

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
struct Large_ap_int {
  static_assert(bits > 64);

  using Operand = std::int64_t;
  using Unsigned_operand = std::uint64_t;

  constexpr Large_ap_int() : v_{0} {};
  constexpr Large_ap_int(const Large_ap_int&) = default;
  constexpr Large_ap_int& operator=(const Large_ap_int&) = default;

  constexpr explicit Large_ap_int(Operand);
  constexpr explicit Large_ap_int(std::string_view);

  constexpr bool operator==(const Large_ap_int&) const;
  constexpr bool operator!=(const Large_ap_int&) const;
  constexpr bool operator<(const Large_ap_int&) const;
  constexpr bool operator<=(const Large_ap_int&) const;
  constexpr bool operator>(const Large_ap_int&) const;
  constexpr bool operator>=(const Large_ap_int&) const;

  constexpr bool operator==(Operand) const;
  constexpr bool operator!=(Operand) const;
  constexpr bool operator<(Operand) const;
  constexpr bool operator<=(Operand) const;
  constexpr bool operator>(Operand) const;
  constexpr bool operator>=(Operand) const;

  constexpr Large_ap_int operator+() const;
  constexpr Large_ap_int operator-() const;
  constexpr Large_ap_int operator~() const;
  constexpr Large_ap_int& operator++();
  constexpr Large_ap_int operator++(int);
  constexpr Large_ap_int& operator--();
  constexpr Large_ap_int operator--(int);

  constexpr Large_ap_int& operator+=(const Large_ap_int&);
  constexpr Large_ap_int& operator-=(const Large_ap_int&);
  constexpr Large_ap_int& operator*=(const Large_ap_int&);
  constexpr Large_ap_int& operator/=(const Large_ap_int&);
  constexpr Large_ap_int& operator%=(const Large_ap_int&);
  constexpr Large_ap_int& operator&=(const Large_ap_int&);
  constexpr Large_ap_int& operator|=(const Large_ap_int&);
  constexpr Large_ap_int& operator^=(const Large_ap_int&);

  constexpr Large_ap_int operator+(const Large_ap_int&) const;
  constexpr Large_ap_int operator-(const Large_ap_int&) const;
  constexpr Large_ap_int operator*(const Large_ap_int&)const;
  constexpr Large_ap_int operator/(const Large_ap_int&) const;
  constexpr Large_ap_int operator%(const Large_ap_int&) const;
  constexpr Large_ap_int operator&(const Large_ap_int&)const;
  constexpr Large_ap_int operator|(const Large_ap_int&) const;
  constexpr Large_ap_int operator^(const Large_ap_int&) const;

  constexpr Large_ap_int& operator+=(Operand);
  constexpr Large_ap_int& operator-=(Operand);
  constexpr Large_ap_int& operator*=(Operand);
  constexpr Large_ap_int& operator/=(Operand);
  constexpr Large_ap_int& operator%=(Operand);
  constexpr Large_ap_int& operator&=(Unsigned_operand);
  constexpr Large_ap_int& operator|=(Unsigned_operand);
  constexpr Large_ap_int& operator^=(Unsigned_operand);
  constexpr Large_ap_int& operator<<=(Unsigned_operand);
  constexpr Large_ap_int& operator>>=(Unsigned_operand);

  constexpr Large_ap_int operator+(Operand) const;
  constexpr Large_ap_int operator-(Operand) const;
  constexpr Large_ap_int operator*(Operand) const;
  constexpr Large_ap_int operator/(Operand) const;
  constexpr Large_ap_int operator%(Operand) const;
  constexpr Large_ap_int operator&(Unsigned_operand) const;
  constexpr Large_ap_int operator|(Unsigned_operand) const;
  constexpr Large_ap_int operator^(Unsigned_operand) const;
  constexpr Large_ap_int operator<<(Unsigned_operand) const;
  constexpr Large_ap_int operator>>(Unsigned_operand) const;

 private:
  using Word = std::uint64_t;
  static constexpr std::size_t bits_per_word_ = sizeof(Word) * CHAR_BIT;
  static constexpr std::size_t last_word_bits =
      ((bits - 1) % bits_per_word_) + 1;
  static constexpr std::size_t words_ = 1 + (bits - 1) / bits_per_word_;
  static_assert(words_ >= 2);

  std::array<Word, words_> v_;

  static constexpr std::size_t which_word(std::size_t bit_pos) {
    return bit_pos / bits_per_word_;
  }

  static constexpr Word which_bit(std::size_t bit_pos) {
    return bit_pos % bits_per_word_;
  }

  static constexpr Word mask_bit(std::size_t bit_pos) {
    return Word(1) << which_bit(bit_pos);
  }

  constexpr const Word& get_word(std::size_t bit_pos) const {
    return v_[which_word(bit_pos)];
  }

  constexpr Word& get_word(std::size_t bit_pos) {
    return v_[which_word(bit_pos)];
  }

  constexpr bool get_bit(std::size_t bit_pos) const {
    assert(bit_pos < bits && "Bit position out of bounds!");
    return (mask_bit(bit_pos) & get_word(bit_pos)) != 0;
  }

  constexpr void set_bit(std::size_t bit_pos) {
    assert(bit_pos < bits && "Bit position out of bounds!");
    get_word(bit_pos) |= mask_bit(bit_pos);
  }

  static constexpr Word low_half(Word v) {
    auto mask = (~(Word)0) >> (Word)(bits_per_word_ / 2);
    return v & mask;
  }

  static constexpr Word high_half(Word v) { return v >> (bits_per_word_ / 2); }

  constexpr bool is_negative() const { return get_bit(bits - 1); }
  constexpr const Word& operator[](std::size_t i) const { return v_[i]; }
  constexpr Word& operator[](std::size_t i) { return v_[i]; }

  constexpr int compare(const Large_ap_int& rhs) const;
  constexpr int compare(Operand rhs) const;

  constexpr Large_ap_int& clear_unused_bits() {
    constexpr Word word_max = ~Word(0);
    constexpr Word mask = word_max >> (bits_per_word_ - last_word_bits);

    v_.back() &= mask;
    return *this;
  }

  constexpr Large_ap_int& fill_unused_bits() {
    constexpr Word word_max = ~Word(0);
    constexpr Word mask = word_max >> (bits_per_word_ - last_word_bits);

    v_.back() |= ~mask;
    return *this;
  }

  constexpr Large_ap_int unsigned_mul(Word) const;
  constexpr std::size_t count_leading_zeros() const;
  constexpr std::tuple<Large_ap_int, Large_ap_int> udivmod(
      const Large_ap_int&) const;
};

template <std::size_t bits>
constexpr Large_ap_int<bits>::Large_ap_int(Operand v) : v_{0} {
  if (v < 0) {
    v_[0] = -v;
    for (std::size_t i = 1; i < words_; ++i) {
      v_[i] = 0;
    }
    *this = -(*this);
  } else {
    v_[0] = v;
    for (std::size_t i = 1; i < words_; ++i) {
      v_[i] = 0;
    }
  }
}

template <std::size_t bits>
constexpr Large_ap_int<bits>::Large_ap_int(std::string_view v) : v_{0} {
  if (v.empty()) {
    return;
  }

  auto ite = v.begin();
  bool neg = *ite == '-';
  if (neg) {
    ++ite;
  }

  while (ite != v.end()) {
    char c = *ite++;
    if (c < '0' || c > '9') {
      break;
    }

    *this *= 10;
    *this += c - '0';
  }

  if (neg) {
    *this = -(*this);
  }
}

// Compares two values, returns -1 if lhs < rhs, 0 if they are equal, or 1 if
// lhs > rhs.
template <std::size_t bits>
constexpr int Large_ap_int<bits>::compare(const Large_ap_int<bits>& rhs) const {
  bool l_neg = is_negative();
  bool r_neg = rhs.is_negative();

  if (l_neg != r_neg) {
    return l_neg ? -1 : 1;
  }

  std::size_t parts = words_;
  while (parts) {
    parts--;
    if (v_[parts] != rhs[parts]) {
      return (v_[parts] > rhs[parts]) ? 1 : -1;
    }
  }
  return 0;
}

template <std::size_t bits>
constexpr int Large_ap_int<bits>::compare(Operand rhs) const {
  bool l_neg = is_negative();
  bool r_neg = rhs < 0;

  if (l_neg != r_neg) {
    return l_neg ? -1 : 1;
  }

  if (l_neg) {
    Word last_word_cmp = (1 << last_word_bits) - 1;
    if (v_.back() != last_word_cmp) {
      return -1;
    }

    std::size_t parts = words_ - 2;
    while (parts > 0) {
      if (v_[parts] != std::numeric_limits<Word>::max()) {
        return -1;
      }
    }
    if (v_[0] == Word(rhs)) {
      return 0;
    }
    return v_[0] > Word(rhs) ? 1 : -1;
  } else {
    std::size_t parts = words_ - 1;
    while (parts > 0) {
      if (v_[parts] != 0) {
        return 1;
      }
      --parts;
    }
    if (v_[0] == Word(rhs)) {
      return 0;
    }
    return v_[0] > Word(rhs) ? 1 : -1;
  }
}

template <std::size_t bits>
constexpr bool Large_ap_int<bits>::operator==(
    const Large_ap_int<bits>& rhs) const {
  return compare(rhs) == 0;
}

template <std::size_t bits>
constexpr bool Large_ap_int<bits>::operator!=(
    const Large_ap_int<bits>& rhs) const {
  return compare(rhs) != 0;
}

template <std::size_t bits>
constexpr bool Large_ap_int<bits>::operator<(
    const Large_ap_int<bits>& rhs) const {
  return compare(rhs) < 0;
}

template <std::size_t bits>
constexpr bool Large_ap_int<bits>::operator<=(
    const Large_ap_int<bits>& rhs) const {
  return compare(rhs) <= 0;
}

template <std::size_t bits>
constexpr bool Large_ap_int<bits>::operator>(
    const Large_ap_int<bits>& rhs) const {
  return compare(rhs) > 0;
}

template <std::size_t bits>
constexpr bool Large_ap_int<bits>::operator>=(
    const Large_ap_int<bits>& rhs) const {
  return compare(rhs) >= 0;
}

template <std::size_t bits>
constexpr bool Large_ap_int<bits>::operator==(Operand rhs) const {
  return compare(rhs) == 0;
}

template <std::size_t bits>
constexpr bool Large_ap_int<bits>::operator!=(Operand rhs) const {
  return compare(rhs) != 0;
}

template <std::size_t bits>
constexpr bool Large_ap_int<bits>::operator<(Operand rhs) const {
  return compare(rhs) < 0;
}

template <std::size_t bits>
constexpr bool Large_ap_int<bits>::operator<=(Operand rhs) const {
  return compare(rhs) <= 0;
}

template <std::size_t bits>
constexpr bool Large_ap_int<bits>::operator>(Operand rhs) const {
  return compare(rhs) > 0;
}

template <std::size_t bits>
constexpr bool Large_ap_int<bits>::operator>=(Operand rhs) const {
  return compare(rhs) >= 0;
}

// ************************** UNARY OPERATORS ************************** //
template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator+() const {
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator-() const {
  Large_ap_int<bits> result = ~(*this);
  ++result;
  return result;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator~() const {
  Large_ap_int<bits> result;

  for (std::size_t i = 0; i < words_; ++i) {
    result.v_[i] = ~v_[i];
  }
  return result.clear_unused_bits();
}

template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator++() {
  *this += 1;
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator++(int) {
  auto tmp = *this;
  *this += 1;
  return tmp;
}

template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator--() {
  *this -= 1;
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator--(int) {
  auto tmp = *this;
  *this -= 1;
  return tmp;
}

// ************************** ADDITION ************************** //
template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator+=(Operand rhs) {
  if (rhs == std::numeric_limits<Operand>::min()) {
    *this -= std::numeric_limits<Operand>::max();
    return *this -= 1;
  }

  if (rhs < 0) {
    return *this -= -rhs;
  }

  Word rhs_w = rhs;
  for (auto& v : v_) {
    v += rhs_w;
    if (v >= rhs_w) {
      break;
    }
    rhs_w = 1;
  }
  clear_unused_bits();
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator+=(
    const Large_ap_int& rhs) {
  bool c = false;
  for (std::size_t i = 0; i < words_; ++i) {
    auto l = v_[i];
    if (c) {
      v_[i] += rhs[i] + 1;
      c = v_[i] <= l;
    } else {
      v_[i] += rhs[i];
      c = v_[i] < l;
    }
  }
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator+(Operand rhs) const {
  return Large_ap_int<bits>(*this) += rhs;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator+(
    const Large_ap_int& rhs) const {
  return Large_ap_int<bits>(*this) += rhs;
}

// ************************** SUBTRACTION ************************** //
template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator-=(Operand rhs) {
  if (rhs == std::numeric_limits<Operand>::min()) {
    *this += std::numeric_limits<Operand>::max();
    return *this += 1;
  }

  if (rhs < 0) {
    return *this += -rhs;
  }

  Word rhs_w = rhs;
  for (auto& v : v_) {
    auto tmp = v;
    v -= rhs_w;
    if (rhs_w <= tmp) {
      break;
    }
    rhs_w = 1;
  }

  clear_unused_bits();
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator-=(
    const Large_ap_int& rhs) {
  bool c = false;
  for (std::size_t i = 0; i < words_; ++i) {
    auto l = v_[i];
    if (c) {
      v_[i] -= rhs[i] + 1;
      c = v_[i] >= l;
    } else {
      v_[i] -= rhs[i];
      c = v_[i] > l;
    }
  }
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator-(Operand rhs) const {
  return Large_ap_int<bits>(*this) -= rhs;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator-(
    const Large_ap_int& rhs) const {
  return Large_ap_int<bits>(*this) -= rhs;
}

// ************************** MULTIPLICATION ************************** //

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::unsigned_mul(Word rhs) const {
  assert(!is_negative());
  Large_ap_int<bits> result{0};

  Word carry = 0;
  for (std::size_t i = 0; i < words_; ++i) {
    Word low = 0, mid = 0, high = 0;
    auto src_part = v_[i];
    // [ LOW, HIGH ] = MULTIPLIER * SRC[i] + DST[i] + CARRY.

    if (src_part == 0 || rhs == 0) {
      low = carry;
      high = 0;
    } else {
      low = low_half(src_part) * low_half(rhs);
      high = high_half(src_part) * high_half(rhs);

      mid = low_half(src_part) * high_half(rhs);
      high += high_half(mid);
      mid <<= bits_per_word_ / 2;
      if (low + mid < low) {
        ++high;
      }
      low += mid;

      mid = high_half(src_part) * low_half(rhs);
      high += high_half(mid);
      mid <<= bits_per_word_ / 2;
      if (low + mid < low) {
        ++high;
      }
      low += mid;

      if (low + carry < low) {
        high++;
      }
      low += carry;
    }
    result[i] = low;
    carry = high;
  }

  return result.clear_unused_bits();
}

template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator*=(Operand rhs) {
  if (rhs == std::numeric_limits<Operand>::min()) {
    *this *= 2;
    return *this *= std::numeric_limits<Operand>::min() / 2;
  }

  if (rhs < 0) {
    (*this) = -(*this);
    rhs = -rhs;
  }
  bool neg = is_negative();
  if (neg) {
    *this = -*this;
  }

  *this = unsigned_mul(rhs);

  if (neg) {
    *this = -*this;
  }

  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator*=(
    const Large_ap_int& rhs) {
  *this = *this * rhs;
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator*(Operand rhs) const {
  return Large_ap_int<bits>(*this) *= rhs;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator*(
    const Large_ap_int& rhs) const {
  Large_ap_int<bits> a{*this};
  Large_ap_int<bits> b{rhs};

  bool neg = false;

  if (a.is_negative()) {
    neg = !neg;
    a = -a;
  }

  if (b.is_negative()) {
    neg = !neg;
    b = -b;
  }

  Large_ap_int<bits> result{0};

  for (std::size_t i = 0; i < words_; ++i) {
    auto tmp = a.unsigned_mul(b[i]);
    tmp <<= (bits_per_word_ * i);
    result += tmp;
  }

  if (neg) {
    result = -result;
  }
  return result;
}

// ************************** DIVISION ************************** //

template <std::size_t bits>
constexpr std::size_t Large_ap_int<bits>::count_leading_zeros() const {
  std::size_t result = 0;

  if (v_.back() != 0) {
    constexpr Word mask = Word(1) << (last_word_bits - 1);
    auto w = v_.back();
    while (1) {
      if ((w & mask) != 0) {
        break;
      }
      ++result;
      w <<= 1;
    }
  } else {
    result += last_word_bits;

    for (int i = (int)words_ - 2; i >= 0; --i) {
      if (v_[i] == 0) {
        result += bits_per_word_;
      } else {
        constexpr Word mask = Word(1) << (bits_per_word_ - 1);
        auto w = v_[i];
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

template <std::size_t bits>
constexpr std::tuple<Large_ap_int<bits>, Large_ap_int<bits>>
Large_ap_int<bits>::udivmod(const Large_ap_int<bits>& denum) const {
  // Good old long division

  if (denum > *this) {
    return std::make_tuple(Large_ap_int<bits>{0}, *this);
  }

  auto divisor = denum;

  auto num_leadind_z = count_leading_zeros();
  auto den_leadind_z = denum.count_leading_zeros();
  std::int64_t shift = std::int64_t(den_leadind_z - num_leadind_z);

  Large_ap_int<bits> result{0};
  auto remainder = *this;

  divisor <<= shift;

  while (shift >= 0) {
    if (remainder < denum) {
      break;
    }
    auto comp = divisor.compare(remainder);
    if (comp <= 0) {
      remainder -= divisor;
      result.set_bit(shift);
      if (comp == 0) {
        break;
      }
    }
    --shift;
    divisor >>= 1;
  }

  return std::make_tuple(result, remainder);
}

template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator/=(Operand rhs) {
  return *this /= Large_ap_int<bits>(rhs);
}

template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator/=(
    const Large_ap_int& rhs) {
  bool neg = false;

  if (is_negative()) {
    neg = !neg;
    *this = -*this;
  }

  auto rhs_v = rhs;
  if (rhs_v < 0) {
    neg = !neg;
    rhs_v = -rhs_v;
  }

  Large_ap_int<bits> rem{0};
  std::tie(*this, rem) = udivmod(rhs_v);

  if (neg) {
    *this = -*this;
  }

  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator/(Operand rhs) const {
  return Large_ap_int<bits>(*this) /= rhs;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator/(
    const Large_ap_int& rhs) const {
  return Large_ap_int<bits>(*this) /= rhs;
}

// ************************** MODULO ************************** //

template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator%=(Operand rhs) {
  return *this %= Large_ap_int<bits>(rhs);
}

template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator%=(
    const Large_ap_int& rhs) {
  bool neg = false;

  if (is_negative()) {
    neg = !neg;
    *this = -*this;
  }

  auto rhs_v = rhs;
  if (rhs_v < 0) {
    neg = !neg;
    rhs_v = -rhs_v;
  }

  Large_ap_int<bits> div{0};
  std::tie(div, *this) = udivmod(rhs_v);

  if (neg) {
    *this = -*this;
  }

  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator%(Operand rhs) const {
  return Large_ap_int<bits>(*this) %= rhs;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator%(
    const Large_ap_int& rhs) const {
  return Large_ap_int<bits>(*this) %= rhs;
}

// ************************** BINARY AND ************************** //

template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator&=(
    Unsigned_operand rhs) {
  v_[0] &= rhs;
  for (std::size_t i = 1; i < words_; ++i) {
    v_[i] = 0;
  }
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator&=(
    const Large_ap_int& rhs) {
  for (std::size_t i = 0; i < words_; ++i) {
    v_[i] &= rhs.v_[i];
  }
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator&(
    Unsigned_operand rhs) const {
  return Large_ap_int<bits>(*this) &= rhs;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator&(
    const Large_ap_int& rhs) const {
  return Large_ap_int<bits>(*this) &= rhs;
}

// ************************** BINARY OR ************************** //

template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator|=(
    Unsigned_operand rhs) {
  v_[0] |= rhs;
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator|=(
    const Large_ap_int& rhs) {
  for (std::size_t i = 0; i < words_; ++i) {
    v_[i] |= rhs.v_[i];
  }
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator|(
    Unsigned_operand rhs) const {
  return Large_ap_int<bits>(*this) |= rhs;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator|(
    const Large_ap_int& rhs) const {
  return Large_ap_int<bits>(*this) |= rhs;
}

// ************************** BINARY XOR ************************** //

template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator^=(
    Unsigned_operand rhs) {
  v_[0] ^= rhs;
  for (std::size_t i = 1; i < words_; ++i) {
    v_[i] ^= Word(0);
  }
  clear_unused_bits();
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator^=(
    const Large_ap_int& rhs) {
  for (std::size_t i = 0; i < words_; ++i) {
    v_[i] ^= rhs.v_[i];
  }
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator^(
    Unsigned_operand rhs) const {
  return Large_ap_int<bits>(*this) ^= rhs;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator^(
    const Large_ap_int& rhs) const {
  return Large_ap_int<bits>(*this) ^= rhs;
}

// ************************** Output to stream ************************** //
template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator<<=(
    Unsigned_operand rhs) {
  if (rhs == 0) {
    return *this;
  }
  std::size_t word_shift = std::min(rhs / bits_per_word_, words_);
  std::size_t bit_shift = rhs % bits_per_word_;

  auto w = words_;
  while (w-- > word_shift) {
    v_[w] = v_[w - word_shift] << bit_shift;
    if (w > word_shift) {
      v_[w] |= v_[w - word_shift - 1] >> (bits_per_word_ - bit_shift);
    }
  }
  for (std::size_t i = 0; i < word_shift; ++i) {
    v_[i] = 0;
  }
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator<<(
    Unsigned_operand rhs) const {
  return Large_ap_int<bits>(*this) <<= rhs;
}

template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator>>=(
    Unsigned_operand rhs) {
  bool neg = is_negative();

  if (neg) {
    fill_unused_bits();
  }

  std::size_t word_shift = std::min(rhs / bits_per_word_, words_);
  std::size_t bit_shift = rhs % bits_per_word_;

  for (std::size_t w = 0; w < (words_ - word_shift); ++w) {
    v_[w] = v_[w + word_shift] >> bit_shift;
    if (w + word_shift + 1 < words_) {
      v_[w] |= v_[w + word_shift + 1] << (bits_per_word_ - bit_shift);
    }
  }

  if (neg) {
    for (std::size_t w = words_ - word_shift; w < words_; ++w) {
      v_[w] = ~Word(0);
    }

    Word mask = ~((1 << (bits_per_word_ - bit_shift)) - 1);
    v_[words_ - word_shift - 1] |= mask;
    clear_unused_bits();
  }

  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator>>(
    Unsigned_operand rhs) const {
  return Large_ap_int<bits>(*this) >>= rhs;
}

}  // namespace vecpp

#endif