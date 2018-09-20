//  Copyright 2018 Francois Chabot
//  (francois.chabot.dev@gmail.com)
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef VECPP_AP_INT_SMALL_INCLUDED_H
#define VECPP_AP_INT_SMALL_INCLUDED_H

#include <climits>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <type_traits>

namespace vecpp {

namespace detail {
template <std::size_t bits, bool is_signed, typename Enable = void>
struct Small_storage_selector;

template <std::size_t bits>
struct Small_storage_selector<bits, true, std::enable_if_t<(bits <= 8)>> {
  using type = std::int8_t;
};

template <std::size_t bits>
struct Small_storage_selector<bits, true,
                              std::enable_if_t<(bits > 8 && bits <= 16)>> {
  using type = std::int16_t;
};

template <std::size_t bits>
struct Small_storage_selector<bits, true,
                              std::enable_if_t<(bits > 16 && bits <= 32)>> {
  using type = std::int32_t;
};

template <std::size_t bits>
struct Small_storage_selector<bits, true,
                              std::enable_if_t<(bits > 32 && bits <= 64)>> {
  using type = std::int64_t;
};

template <std::size_t bits>
struct Small_storage_selector<bits, false, std::enable_if_t<(bits <= 8)>> {
  using type = std::uint8_t;
};

template <std::size_t bits>
struct Small_storage_selector<bits, false,
                              std::enable_if_t<(bits > 8 && bits <= 16)>> {
  using type = std::uint16_t;
};

template <std::size_t bits>
struct Small_storage_selector<bits, false,
                              std::enable_if_t<(bits > 16 && bits <= 32)>> {
  using type = std::uint32_t;
};

template <std::size_t bits>
struct Small_storage_selector<bits, false,
                              std::enable_if_t<(bits > 32 && bits <= 64)>> {
  using type = std::uint64_t;
};
}

// For Integer values < than the system's representable integers,
// we just just bitfields, and let the compiler take it from there.
template <std::size_t bits, bool is_signed>
struct Small_ap_int {
  static_assert(bits > 1 && bits <= 64);

 private:
  using Storage =
      typename detail::Small_storage_selector<bits, is_signed>::type;
  using Self = Small_ap_int;  // This is just so that we can write one-liners.
  using Op = Storage;
  using UOp = std::make_signed_t<Op>;

 public:
  constexpr Small_ap_int() = default;
  constexpr Small_ap_int(Storage v) : v_(v) {}
  constexpr Small_ap_int(const Small_ap_int&) = default;
  constexpr Small_ap_int& operator=(const Small_ap_int&) = default;

  constexpr bool operator==(const Self& rhs) const { return v_ == rhs.v_; }
  constexpr bool operator!=(const Self& rhs) const { return v_ != rhs.v_; }
  constexpr bool operator>(const Self& rhs) const { return v_ > rhs.v_; }
  constexpr bool operator<(const Self& rhs) const { return v_ < rhs.v_; }
  constexpr bool operator>=(const Self& rhs) const { return v_ >= rhs.v_; }
  constexpr bool operator<=(const Self& rhs) const { return v_ <= rhs.v_; }

  constexpr bool operator==(const Op& rhs) const { return v_ == rhs; }
  constexpr bool operator!=(const Op& rhs) const { return v_ != rhs; }
  constexpr bool operator>(const Op& rhs) const { return v_ > rhs; }
  constexpr bool operator<(const Op& rhs) const { return v_ < rhs; }
  constexpr bool operator>=(const Op& rhs) const { return v_ >= rhs; }
  constexpr bool operator<=(const Op& rhs) const { return v_ <= rhs; }

  constexpr Self operator+() const { return +v_; }
  constexpr Self operator-() const { return -v_; }
  constexpr Self operator~() const { return ~v_; }
  constexpr Self& operator++() { return ++v_; }
  constexpr Self operator++(int) { return v_++; }
  constexpr Self& operator--() { return --v_; }
  constexpr Self operator--(int) { return v_--; }

  constexpr Self& operator+=(const Self& rhs) { return v_ += rhs.v_; }
  constexpr Self& operator-=(const Self& rhs) { return v_ -= rhs.v_; }
  constexpr Self& operator*=(const Self& rhs) { return v_ *= rhs.v_; }
  constexpr Self& operator/=(const Self& rhs) { return v_ /= rhs.v_; }
  constexpr Self& operator%=(const Self& rhs) { return v_ %= rhs.v_; }
  constexpr Self& operator&=(const Self& rhs) { return v_ &= rhs.v_; }
  constexpr Self& operator|=(const Self& rhs) { return v_ |= rhs.v_; }
  constexpr Self& operator^=(const Self& rhs) { return v_ ^= rhs.v_; }

  constexpr Self operator+(const Self& rhs) const { return v_ + rhs.v_; }
  constexpr Self operator-(const Self& rhs) const { return v_ - rhs.v_; }
  constexpr Self operator*(const Self& rhs) const { return v_ * rhs.v_; }
  constexpr Self operator/(const Self& rhs) const { return v_ / rhs.v_; }
  constexpr Self operator%(const Self& rhs) const { return v_ % rhs.v_; }
  constexpr Self operator&(const Self& rhs) const { return v_ & rhs.v_; }
  constexpr Self operator|(const Self& rhs) const { return v_ | rhs.v_; }
  constexpr Self operator^(const Self& rhs) const { return v_ ^ rhs.v_; }

  constexpr Self& operator+=(const Op& rhs) { return v_ += v_; }
  constexpr Self& operator-=(const Op& rhs) { return v_ -= v_; }
  constexpr Self& operator*=(const Op& rhs) { return v_ *= v_; }
  constexpr Self& operator/=(const Op& rhs) { return v_ /= v_; }
  constexpr Self& operator%=(const Op& rhs) { return v_ %= v_; }
  constexpr Self& operator&=(const Op& rhs) { return v_ &= v_; }
  constexpr Self& operator|=(const Op& rhs) { return v_ |= v_; }
  constexpr Self& operator^=(const Op& rhs) { return v_ ^= v_; }

  constexpr Self operator+(const Op& rhs) const { return v_ + rhs; }
  constexpr Self operator-(const Op& rhs) const { return v_ - rhs; }
  constexpr Self operator*(const Op& rhs) const { return v_ * rhs; }
  constexpr Self operator/(const Op& rhs) const { return v_ / rhs; }
  constexpr Self operator%(const Op& rhs) const { return v_ % rhs; }
  constexpr Self operator&(const Op& rhs) const { return v_ & rhs; }
  constexpr Self operator|(const Op& rhs) const { return v_ | rhs; }
  constexpr Self operator^(const Op& rhs) const { return v_ ^ rhs; }

  constexpr Self operator<<(Op arg) const { return v_ << arg; }
  constexpr Self operator>>(Op arg) const { return v_ >> arg; }

 private:
  Storage v_ : bits;

  template <std::size_t b, bool s>
  friend std::ostream& operator<<(std::ostream& stream, Small_ap_int<b, s> val);
};

template <std::size_t bits, bool is_signed>
std::ostream& operator<<(std::ostream& stream,
                         Small_ap_int<bits, is_signed> val) {
  using Storage = typename Small_ap_int<bits, is_signed>::Storage;
  if
    constexpr(std::is_same_v<Storage, std::int8_t> ||
              std::is_same_v<Storage, std::uint8_t>) {
      return stream << (int)val.v_;
    }
  else {
    return stream << val.v_;
  }
}
}

#endif