//  Copyright 2018 Francois Chabot
//  (francois.chabot.dev@gmail.com)
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef VECPP_AP_INT_INCLUDED_H
#define VECPP_AP_INT_INCLUDED_H

#include "vecpp/ap_math/ap_int/small.h"
#include "vecpp/ap_math/ap_int/large.h"

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
    using type = Large_ap_int<bits>;
  };

  template<std::size_t bits>
  struct Ap_int_selector<bits, std::enable_if_t<(bits <= 64)>> {
    using type = Small_ap_int<bits>;
  };

  template<std::size_t bits>
  using Ap_int = typename Ap_int_selector<bits>::type;
}

#endif