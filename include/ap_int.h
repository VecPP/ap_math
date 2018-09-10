//  Copyright 2018 Francois Chabot
//  (francois.chabot.dev@gmail.com)
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef VECPP_AP_INT_INCLUDED_H
#define VECPP_AP_INT_INCLUDED_H

#include "ap_int/simple.h"
#include "ap_int/composed.h"

#include <cstddef>
#include <cstdint>
#include <climits>
#include <type_traits>

namespace vecpp {

  // Selection process for Ap_int implementation.
  // Rules:
  // 1. If there is a native type, use it
  // 2. If we can make it fit within a native type, use the smallest
  //    available native type as storage of a "simple" type.
  // 3. Use the composed type.
  
  template<std::size_t bits, typename Enable=void>
  struct Ap_int_selector {
    using type = Composed_ap_int_impl<bits>;
  };

  template<std::size_t bits>
  struct Ap_int_selector<bits, std::enable_if_t<(bits < 8)>> {
    using type = Simple_ap_int_impl<bits, std::uint8_t>;
  };

  template<>
  struct Ap_int_selector<8, void> {
    using type = std::int8_t;
  };

  template<std::size_t bits>
  struct Ap_int_selector<bits, std::enable_if_t<(bits > 8 && bits < 16)>> {
    using type = Simple_ap_int_impl<bits, std::uint16_t>;
  };

  template<>
  struct Ap_int_selector<16, void> {
    using type = std::int16_t;
  };

  template<std::size_t bits>
  struct Ap_int_selector<bits, std::enable_if_t<(bits > 16 && bits < 32)>> {
    using type = Simple_ap_int_impl<bits, std::uint32_t>;
  };

  template<>
  struct Ap_int_selector<32, void> {
    using type = std::int32_t;
  };

  template<std::size_t bits>
  struct Ap_int_selector<bits, std::enable_if_t<(bits > 32 && bits < 64)>> {
    using type = Simple_ap_int_impl<bits, std::uint64_t>;
  };

  template<>
  struct Ap_int_selector<64, void> {
    using type = std::int64_t;
  };

  template<std::size_t bits>
  using Ap_int = typename Ap_int_selector<bits>::type;
}

#endif