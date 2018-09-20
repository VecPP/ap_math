//  Copyright 2018 Francois Chabot
//  (francois.chabot.dev@gmail.com)
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef VECPP_AP_INT_NUMERIC_LIMITS_INCLUDED_H
#define VECPP_AP_INT_NUMERIC_LIMITS_INCLUDED_H

#include "vecpp/ap_math/ap_int/large_signed.h"
//#include "vecpp/ap_math/ap_int/large_unsigned.h"
#include "vecpp/ap_math/ap_int/small.h"

#include <limits>

namespace std {

template <std::size_t bits>
struct numeric_limits<vecpp::Small_ap_int<bits, false>>
    : public std::numeric_limits<
          typename vecpp::detail::Small_storage_selector<bits, false>::type> {
  using T = vecpp::Small_ap_int<bits, false>;

  // is_modulo is inherited from the underlying type here.

  static constexpr int digits = bits;
  static constexpr int digits10 = digits * 643L / 2136;

  static constexpr T min() { 0; }
  static constexpr T lowest() { 0; }
  static constexpr T max() { return ~min(); }
};

template <std::size_t bits>
struct numeric_limits<vecpp::Small_ap_int<bits, true>>
    : public std::numeric_limits<
          typename vecpp::detail::Small_storage_selector<bits, true>::type> {
  using T = vecpp::Small_ap_int<bits, true>;

  // is_modulo is inherited from the underlying type here.

  static constexpr int digits = bits;
  static constexpr int digits10 = digits * 643L / 2136;

  static constexpr T min() { return T{1} << (bits - 1); }
  static constexpr T lowest() { return min(); }
  static constexpr T max() { return ~min(); }
};

template <std::size_t bits>
struct numeric_limits<vecpp::Large_ap_int<bits>>
    : public std::numeric_limits<std::int64_t> {
  using T = vecpp::Large_ap_int<bits>;

  static constexpr bool is_modulo = true;

  static constexpr int digits = bits;
  static constexpr int digits10 = digits * 643L / 2136;

  static constexpr T min() { return T{1} << (bits - 1); }
  static constexpr T lowest() { return min(); }
  static constexpr T max() { return ~min(); }
};

template <std::size_t bits>
struct numeric_limits<vecpp::Large_ap_uint<bits>>
    : public std::numeric_limits<std::uint64_t> {
  using T = vecpp::Large_ap_uint<bits>;

  static constexpr bool is_modulo = true;

  static constexpr int digits = bits;
  static constexpr int digits10 = digits * 643L / 2136;

  static constexpr T min() { return T(0); }
  static constexpr T lowest() { return min(); }
  static constexpr T max() { return ~min(); }
};
}

#endif