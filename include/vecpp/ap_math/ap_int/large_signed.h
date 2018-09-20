//  Copyright 2018 Francois Chabot
//  (francois.chabot.dev@gmail.com)
//
//  See the accompanying LICENSE file for licensing details.

#ifndef VECPP_AP_INT_COMPOSED_INCLUDED_H
#define VECPP_AP_INT_COMPOSED_INCLUDED_H

#include "vecpp/ap_math/ap_int/int_storage.h"

#include <sstream>
#include <string>

namespace vecpp {
// This is really simple. An AP int
template <std::size_t bits>
struct Large_ap_int {
  using Storage = detail::Int_storage<bits, std::uint64_t>;
  using Self = Large_ap_int;

  Large_ap_int(){};
  constexpr Large_ap_int(const Self&) = default;
  constexpr Large_ap_int& operator=(const Self&) = default;

  constexpr explicit Large_ap_int(std::int64_t);
  constexpr explicit Large_ap_int(std::string_view);

  constexpr int compare(const Large_ap_int&) const;
  constexpr bool operator==(const Self& r) const { return compare(r) == 0; }
  constexpr bool operator!=(const Self& r) const { return compare(r) != 0; }
  constexpr bool operator<(const Self& r) const { return compare(r) < 0; }
  constexpr bool operator<=(const Self& r) const { return compare(r) <= 0; }
  constexpr bool operator>(const Self& r) const { return compare(r) > 0; }
  constexpr bool operator>=(const Self& r) const { return compare(r) >= 0; }

  constexpr int compare(std::int64_t) const;
  constexpr bool operator==(std::int64_t r) const { return compare(r) == 0; }
  constexpr bool operator!=(std::int64_t r) const { return compare(r) != 0; }
  constexpr bool operator<(std::int64_t r) const { return compare(r) < 0; }
  constexpr bool operator<=(std::int64_t r) const { return compare(r) <= 0; }
  constexpr bool operator>(std::int64_t r) const { return compare(r) > 0; }
  constexpr bool operator>=(std::int64_t r) const { return compare(r) >= 0; }

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

  constexpr Large_ap_int& operator+=(std::int64_t);
  constexpr Large_ap_int& operator-=(std::int64_t);
  constexpr Large_ap_int& operator*=(std::int64_t);
  constexpr Large_ap_int& operator/=(std::int64_t);
  constexpr Large_ap_int& operator%=(std::int64_t);
  constexpr Large_ap_int& operator&=(std::uint64_t);
  constexpr Large_ap_int& operator|=(std::uint64_t);
  constexpr Large_ap_int& operator^=(std::uint64_t);
  constexpr Large_ap_int& operator<<=(std::uint64_t);
  constexpr Large_ap_int& operator>>=(std::uint64_t);

  constexpr Large_ap_int operator+(std::int64_t) const;
  constexpr Large_ap_int operator-(std::int64_t) const;
  constexpr Large_ap_int operator*(std::int64_t) const;
  constexpr Large_ap_int operator/(std::int64_t) const;
  constexpr Large_ap_int operator%(std::int64_t) const;
  constexpr Large_ap_int operator&(std::uint64_t) const;
  constexpr Large_ap_int operator|(std::uint64_t) const;
  constexpr Large_ap_int operator^(std::uint64_t) const;
  constexpr Large_ap_int operator<<(std::uint64_t) const;
  constexpr Large_ap_int operator>>(std::uint64_t) const;

  Storage data_;

 private:
  using Word = typename Storage::Word;

  constexpr bool is_negative() const { return data_.get_bit(bits - 1); }

  template <std::size_t b>
  friend std::ostream& operator<<(std::ostream&, const Large_ap_int<b>&);
};

template <std::size_t bits>
constexpr Large_ap_int<bits>::Large_ap_int(std::int64_t v) : data_{0} {
  if (v < 0) {
    data_[0] = Word(v);
    for (std::size_t i = 1; i < data_.words; ++i) {
      data_[i] = ~Word(0);
    }

    data_.clear_unused_bits();
  } else {
    data_[0] = Word(v);
  }
}

template <std::size_t bits>
constexpr Large_ap_int<bits>::Large_ap_int(std::string_view v) : data_{0} {
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

  data_.clear_unused_bits();
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

  return data_.compare(rhs.data_);
}

template <std::size_t bits>
constexpr int Large_ap_int<bits>::compare(std::int64_t rhs) const {
  bool l_neg = is_negative();
  bool r_neg = rhs < 0;

  if (l_neg != r_neg) {
    return l_neg ? -1 : 1;
  }

  return data_.compare(Large_ap_int(rhs).data_);
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
  Large_ap_int<bits> result = *this;

  result.data_.invert();

  return result;
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
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator+=(std::int64_t rhs) {
  *this += Large_ap_int(rhs);
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator+=(
    const Large_ap_int& rhs) {
  data_.add(rhs.data_);
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator+(
    std::int64_t rhs) const {
  return Large_ap_int<bits>(*this) += rhs;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator+(
    const Large_ap_int& rhs) const {
  return Large_ap_int<bits>(*this) += rhs;
}

// ************************** SUBTRACTION ************************** //
template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator-=(std::int64_t rhs) {
  *this -= Large_ap_int(rhs);
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator-=(
    const Large_ap_int& rhs) {
  data_.subtract(rhs.data_);
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator-(
    std::int64_t rhs) const {
  return Large_ap_int<bits>(*this) -= rhs;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator-(
    const Large_ap_int& rhs) const {
  return Large_ap_int<bits>(*this) -= rhs;
}

// ************************** MULTIPLICATION ************************** //
template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator*=(std::int64_t rhs) {
  if (rhs == std::numeric_limits<std::int64_t>::min()) {
    *this *= 2;
    return *this *= std::numeric_limits<std::int64_t>::min() / 2;
  }

  if (rhs < 0) {
    (*this) = -(*this);
    rhs = -rhs;
  }
  bool neg = is_negative();
  if (neg) {
    *this = -*this;
  }

  data_.mul(Word(rhs));

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
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator*(
    std::int64_t rhs) const {
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

  for (std::size_t i = 0; i < data_.words; ++i) {
    auto tmp = a;
    tmp.data_.mul(b.data_[i]);

    tmp <<= (data_.bits_per_word * i);
    result += tmp;
  }

  if (neg) {
    result = -result;
  }
  return result;
}

// ************************** DIVISION ************************** //

template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator/=(std::int64_t rhs) {
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

  Storage rem{0};
  std::tie(data_, rem) = data_.udivmod(rhs_v.data_);

  if (neg) {
    *this = -*this;
  }

  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator/(
    std::int64_t rhs) const {
  return Large_ap_int<bits>(*this) /= rhs;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator/(
    const Large_ap_int& rhs) const {
  return Large_ap_int<bits>(*this) /= rhs;
}

// ************************** MODULO ************************** //

template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator%=(std::int64_t rhs) {
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

  Storage div{0};
  std::tie(div, data_) = data_.udivmod(rhs_v.data_);

  if (neg) {
    *this = -*this;
  }

  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator%(
    std::int64_t rhs) const {
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
    const Large_ap_int& rhs) {
  data_.binary_and(rhs.data_);
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator&(
    const Large_ap_int& rhs) const {
  return Large_ap_int<bits>(*this) &= rhs;
}

// ************************** BINARY OR ************************** //

template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator|=(
    const Large_ap_int& rhs) {
  data_.binary_or(rhs.data_);
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator|(
    const Large_ap_int& rhs) const {
  return Large_ap_int<bits>(*this) |= rhs;
}

// ************************** BINARY XOR ************************** //

template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator^=(
    const Large_ap_int& rhs) {
  data_.binary_xor(rhs.data_);
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator^(
    const Large_ap_int& rhs) const {
  return Large_ap_int<bits>(*this) ^= rhs;
}

// ************************** Output to stream ************************** //
template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator<<=(
    std::uint64_t rhs) {
  data_.lshift(rhs);
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator<<(
    std::uint64_t rhs) const {
  return Large_ap_int<bits>(*this) <<= rhs;
}

template <std::size_t bits>
constexpr Large_ap_int<bits>& Large_ap_int<bits>::operator>>=(
    std::uint64_t rhs) {
  data_.rshift(rhs);

  return *this;
}

template <std::size_t bits>
constexpr Large_ap_int<bits> Large_ap_int<bits>::operator>>(
    std::uint64_t rhs) const {
  return Large_ap_int<bits>(*this) >>= rhs;
}

template <std::size_t bits>
std::ostream& operator<<(std::ostream& stream, const Large_ap_int<bits>& num) {
  Large_ap_int<bits> u_num = num;
  if (num.is_negative()) {
    stream << "-";
    u_num = -u_num;
  }

  std::ostringstream gen;

  Large_ap_int<bits> ten{10};
  typename Large_ap_int<bits>::Storage rem;
  while (u_num != 0) {
    std::tie(u_num.data_, rem) = u_num.data_.udivmod(ten.data_);

    gen << char('0' + rem[0]);
  }

  std::string result = gen.str();
  std::reverse(result.begin(), result.end());
  return stream << result;
}

}  // namespace vecpp

#endif