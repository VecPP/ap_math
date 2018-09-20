//  Copyright 2018 Francois Chabot
//  (francois.chabot.dev@gmail.com)
//
//  See the accompanying LICENSE file for licensing details.

#ifndef VECPP_AP_UINT_COMPOSED_INCLUDED_H
#define VECPP_AP_UINT_COMPOSED_INCLUDED_H

#include "vecpp/ap_math/ap_int/int_storage.h"

#include <sstream>
#include <string>

namespace vecpp {
// This is really simple. An AP int
template <std::size_t bits>
struct Large_ap_uint {
  using Storage = detail::Int_storage<bits, std::uint64_t>;
  using Self = Large_ap_uint;

  Large_ap_uint(){};
  constexpr Large_ap_uint(const Self&) = default;
  constexpr Large_ap_uint& operator=(const Self&) = default;

  constexpr explicit Large_ap_uint(std::uint64_t);
  constexpr explicit Large_ap_uint(std::string_view);

  constexpr int compare(const Large_ap_uint&) const;
  constexpr bool operator==(const Self& r) const { return compare(r) == 0; }
  constexpr bool operator!=(const Self& r) const { return compare(r) != 0; }
  constexpr bool operator<(const Self& r) const { return compare(r) < 0; }
  constexpr bool operator<=(const Self& r) const { return compare(r) <= 0; }
  constexpr bool operator>(const Self& r) const { return compare(r) > 0; }
  constexpr bool operator>=(const Self& r) const { return compare(r) >= 0; }

  constexpr int compare(std::uint64_t) const;
  constexpr bool operator==(std::uint64_t r) const { return compare(r) == 0; }
  constexpr bool operator!=(std::uint64_t r) const { return compare(r) != 0; }
  constexpr bool operator<(std::uint64_t r) const { return compare(r) < 0; }
  constexpr bool operator<=(std::uint64_t r) const { return compare(r) <= 0; }
  constexpr bool operator>(std::uint64_t r) const { return compare(r) > 0; }
  constexpr bool operator>=(std::uint64_t r) const { return compare(r) >= 0; }

  constexpr Large_ap_uint operator~() const;
  constexpr Large_ap_uint& operator++();
  constexpr Large_ap_uint operator++(int);
  constexpr Large_ap_uint& operator--();
  constexpr Large_ap_uint operator--(int);

  constexpr Large_ap_uint& operator+=(const Large_ap_uint&);
  constexpr Large_ap_uint& operator-=(const Large_ap_uint&);
  constexpr Large_ap_uint& operator*=(const Large_ap_uint&);
  constexpr Large_ap_uint& operator/=(const Large_ap_uint&);
  constexpr Large_ap_uint& operator%=(const Large_ap_uint&);
  constexpr Large_ap_uint& operator&=(const Large_ap_uint&);
  constexpr Large_ap_uint& operator|=(const Large_ap_uint&);
  constexpr Large_ap_uint& operator^=(const Large_ap_uint&);

  constexpr Large_ap_uint operator+(const Large_ap_uint&) const;
  constexpr Large_ap_uint operator-(const Large_ap_uint&) const;
  constexpr Large_ap_uint operator*(const Large_ap_uint&)const;
  constexpr Large_ap_uint operator/(const Large_ap_uint&) const;
  constexpr Large_ap_uint operator%(const Large_ap_uint&) const;
  constexpr Large_ap_uint operator&(const Large_ap_uint&)const;
  constexpr Large_ap_uint operator|(const Large_ap_uint&) const;
  constexpr Large_ap_uint operator^(const Large_ap_uint&) const;

  constexpr Large_ap_uint& operator+=(std::uint64_t);
  constexpr Large_ap_uint& operator-=(std::uint64_t);
  constexpr Large_ap_uint& operator*=(std::uint64_t);
  constexpr Large_ap_uint& operator/=(std::uint64_t);
  constexpr Large_ap_uint& operator%=(std::uint64_t);
  constexpr Large_ap_uint& operator&=(std::uint64_t);
  constexpr Large_ap_uint& operator|=(std::uint64_t);
  constexpr Large_ap_uint& operator^=(std::uint64_t);
  constexpr Large_ap_uint& operator<<=(std::uint64_t);
  constexpr Large_ap_uint& operator>>=(std::uint64_t);

  constexpr Large_ap_uint operator+(std::uint64_t) const;
  constexpr Large_ap_uint operator-(std::uint64_t) const;
  constexpr Large_ap_uint operator*(std::uint64_t) const;
  constexpr Large_ap_uint operator/(std::uint64_t) const;
  constexpr Large_ap_uint operator%(std::uint64_t) const;
  constexpr Large_ap_uint operator&(std::uint64_t) const;
  constexpr Large_ap_uint operator|(std::uint64_t) const;
  constexpr Large_ap_uint operator^(std::uint64_t) const;
  constexpr Large_ap_uint operator<<(std::uint64_t) const;
  constexpr Large_ap_uint operator>>(std::uint64_t) const;

  Storage data_;

 private:
  using Word = typename Storage::Word;

  constexpr bool is_negative() const { return data_.get_bit(bits - 1); }

  template <std::size_t b>
  friend std::ostream& operator<<(std::ostream&, const Large_ap_uint<b>&);
};

template <std::size_t bits>
constexpr Large_ap_uint<bits>::Large_ap_uint(std::uint64_t v) : data_{0} {
  data_[0] = v;
}

template <std::size_t bits>
constexpr Large_ap_uint<bits>::Large_ap_uint(std::string_view v) : data_{0} {
  if (v.empty()) {
    return;
  }

  auto ite = v.begin();

  while (ite != v.end()) {
    char c = *ite++;
    if (c < '0' || c > '9') {
      break;
    }

    *this *= 10;
    *this += c - '0';
  }

  data_.clear_unused_bits();
}

// Compares two values, returns -1 if lhs < rhs, 0 if they are equal, or 1 if
// lhs > rhs.
template <std::size_t bits>
constexpr int Large_ap_uint<bits>::compare(
    const Large_ap_uint<bits>& rhs) const {
  return data_.compare(rhs.data_);
}

template <std::size_t bits>
constexpr int Large_ap_uint<bits>::compare(std::uint64_t rhs) const {
  return data_.compare(Large_ap_uint(rhs).data_);
}

// ************************** UNARY OPERATORS ************************** //

template <std::size_t bits>
constexpr Large_ap_uint<bits> Large_ap_uint<bits>::operator~() const {
  Large_ap_uint<bits> result = *this;

  result.data_.invert();

  return result;
}

template <std::size_t bits>
constexpr Large_ap_uint<bits>& Large_ap_uint<bits>::operator++() {
  *this += 1;
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_uint<bits> Large_ap_uint<bits>::operator++(int) {
  auto tmp = *this;
  *this += 1;
  return tmp;
}

template <std::size_t bits>
constexpr Large_ap_uint<bits>& Large_ap_uint<bits>::operator--() {
  *this -= 1;
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_uint<bits> Large_ap_uint<bits>::operator--(int) {
  auto tmp = *this;
  *this -= 1;
  return tmp;
}

// ************************** ADDITION ************************** //
template <std::size_t bits>
constexpr Large_ap_uint<bits>& Large_ap_uint<bits>::operator+=(
    std::uint64_t rhs) {
  *this += Large_ap_uint(rhs);
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_uint<bits>& Large_ap_uint<bits>::operator+=(
    const Large_ap_uint& rhs) {
  data_.add(rhs.data_);
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_uint<bits> Large_ap_uint<bits>::operator+(
    std::uint64_t rhs) const {
  return Large_ap_uint<bits>(*this) += rhs;
}

template <std::size_t bits>
constexpr Large_ap_uint<bits> Large_ap_uint<bits>::operator+(
    const Large_ap_uint& rhs) const {
  return Large_ap_uint<bits>(*this) += rhs;
}

// ************************** SUBTRACTION ************************** //
template <std::size_t bits>
constexpr Large_ap_uint<bits>& Large_ap_uint<bits>::operator-=(
    std::uint64_t rhs) {
  *this -= Large_ap_uint(rhs);
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_uint<bits>& Large_ap_uint<bits>::operator-=(
    const Large_ap_uint& rhs) {
  data_.subtract(rhs.data_);
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_uint<bits> Large_ap_uint<bits>::operator-(
    std::uint64_t rhs) const {
  return Large_ap_uint<bits>(*this) -= rhs;
}

template <std::size_t bits>
constexpr Large_ap_uint<bits> Large_ap_uint<bits>::operator-(
    const Large_ap_uint& rhs) const {
  return Large_ap_uint<bits>(*this) -= rhs;
}

// ************************** MULTIPLICATION ************************** //
template <std::size_t bits>
constexpr Large_ap_uint<bits>& Large_ap_uint<bits>::operator*=(
    std::uint64_t rhs) {
  data_.mul(Word(rhs));
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_uint<bits>& Large_ap_uint<bits>::operator*=(
    const Large_ap_uint& rhs) {
  *this = *this * rhs;
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_uint<bits> Large_ap_uint<bits>::operator*(
    std::uint64_t rhs) const {
  return Large_ap_uint<bits>(*this) *= rhs;
}

template <std::size_t bits>
constexpr Large_ap_uint<bits> Large_ap_uint<bits>::operator*(
    const Large_ap_uint& rhs) const {
  Large_ap_uint<bits> a{*this};
  Large_ap_uint<bits> b{rhs};

  Large_ap_uint<bits> result{0};

  for (std::size_t i = 0; i < data_.words; ++i) {
    auto tmp = a;
    tmp.data_.mul(b.data_[i]);

    tmp <<= (data_.bits_per_word * i);
    result += tmp;
  }
  return result;
}

// ************************** DIVISION ************************** //

template <std::size_t bits>
constexpr Large_ap_uint<bits>& Large_ap_uint<bits>::operator/=(
    std::uint64_t rhs) {
  return *this /= Large_ap_uint<bits>(rhs);
}

template <std::size_t bits>
constexpr Large_ap_uint<bits>& Large_ap_uint<bits>::operator/=(
    const Large_ap_uint& rhs) {
  Storage rem{0};
  std::tie(data_, rem) = data_.udivmod(rhs.data_);

  return *this;
}

template <std::size_t bits>
constexpr Large_ap_uint<bits> Large_ap_uint<bits>::operator/(
    std::uint64_t rhs) const {
  return Large_ap_uint<bits>(*this) /= rhs;
}

template <std::size_t bits>
constexpr Large_ap_uint<bits> Large_ap_uint<bits>::operator/(
    const Large_ap_uint& rhs) const {
  return Large_ap_uint<bits>(*this) /= rhs;
}

// ************************** MODULO ************************** //

template <std::size_t bits>
constexpr Large_ap_uint<bits>& Large_ap_uint<bits>::operator%=(
    std::uint64_t rhs) {
  return *this %= Large_ap_uint<bits>(rhs);
}

template <std::size_t bits>
constexpr Large_ap_uint<bits>& Large_ap_uint<bits>::operator%=(
    const Large_ap_uint& rhs) {
  Storage div{0};
  std::tie(div, data_) = data_.udivmod(rhs.data_);
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_uint<bits> Large_ap_uint<bits>::operator%(
    std::uint64_t rhs) const {
  return Large_ap_uint<bits>(*this) %= rhs;
}

template <std::size_t bits>
constexpr Large_ap_uint<bits> Large_ap_uint<bits>::operator%(
    const Large_ap_uint& rhs) const {
  return Large_ap_uint<bits>(*this) %= rhs;
}

// ************************** BINARY AND ************************** //
template <std::size_t bits>
constexpr Large_ap_uint<bits>& Large_ap_uint<bits>::operator&=(
    const Large_ap_uint& rhs) {
  data_.binary_and(rhs.data_);
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_uint<bits> Large_ap_uint<bits>::operator&(
    const Large_ap_uint& rhs) const {
  return Large_ap_uint<bits>(*this) &= rhs;
}

// ************************** BINARY OR ************************** //

template <std::size_t bits>
constexpr Large_ap_uint<bits>& Large_ap_uint<bits>::operator|=(
    const Large_ap_uint& rhs) {
  data_.binary_or(rhs.data_);
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_uint<bits> Large_ap_uint<bits>::operator|(
    const Large_ap_uint& rhs) const {
  return Large_ap_uint<bits>(*this) |= rhs;
}

// ************************** BINARY XOR ************************** //

template <std::size_t bits>
constexpr Large_ap_uint<bits>& Large_ap_uint<bits>::operator^=(
    const Large_ap_uint& rhs) {
  data_.binary_xor(rhs.data_);
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_uint<bits> Large_ap_uint<bits>::operator^(
    const Large_ap_uint& rhs) const {
  return Large_ap_uint<bits>(*this) ^= rhs;
}

// ************************** Output to stream ************************** //
template <std::size_t bits>
constexpr Large_ap_uint<bits>& Large_ap_uint<bits>::operator<<=(
    std::uint64_t rhs) {
  data_.lshift(rhs);
  return *this;
}

template <std::size_t bits>
constexpr Large_ap_uint<bits> Large_ap_uint<bits>::operator<<(
    std::uint64_t rhs) const {
  return Large_ap_uint<bits>(*this) <<= rhs;
}

template <std::size_t bits>
constexpr Large_ap_uint<bits>& Large_ap_uint<bits>::operator>>=(
    std::uint64_t rhs) {
  data_.rshift(rhs);

  return *this;
}

template <std::size_t bits>
constexpr Large_ap_uint<bits> Large_ap_uint<bits>::operator>>(
    std::uint64_t rhs) const {
  return Large_ap_uint<bits>(*this) >>= rhs;
}

template <std::size_t bits>
std::ostream& operator<<(std::ostream& stream, const Large_ap_uint<bits>& num) {
  Large_ap_uint<bits> u_num = num;

  std::ostringstream gen;

  Large_ap_uint<bits> ten{10};
  typename Large_ap_uint<bits>::Storage rem;
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