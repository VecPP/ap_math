//  Copyright 2018 Francois Chabot
//  (francois.chabot.dev@gmail.com)
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef VECPP_AP_INT_INCLUDED_H
#define VECPP_AP_INT_INCLUDED_H

#include "vecpp/ap_math/ap_int/large_signed.h"
#include "vecpp/ap_math/ap_int/large_unsigned.h"
#include "vecpp/ap_math/ap_int/small.h"

#include <climits>
#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace vecpp {

// Selection process for Ap_int implementation.
// Rules:
// 1. If we can make it fit within a native type, use the smallest
//    available native type as storage of a "simple" type.
// 2. Use the composed type.
template <std::size_t bits, typename Enable = void>
struct Ap_int_selector {
  using type = Large_ap_int<bits>;
};

template <std::size_t bits>
struct Ap_int_selector<bits, std::enable_if_t<(bits <= 64)>> {
  using type = Small_ap_int<bits, true>;
};

template <std::size_t bits>
using Ap_int = typename Ap_int_selector<bits>::type;

// Unsigned int
template <std::size_t bits, typename Enable = void>
struct Ap_uint_selector {
  using type = Large_ap_uint<bits>;
};

template <std::size_t bits>
struct Ap_uint_selector<bits, std::enable_if_t<(bits <= 64)>> {
  using type = Small_ap_int<bits, false>;
};

template <std::size_t bits>
using Ap_uint = typename Ap_uint_selector<bits>::type;
}

#endif