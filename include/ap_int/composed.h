//  Copyright 2018 Francois Chabot
//  (francois.chabot.dev@gmail.com)
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef VECPP_AP_INT_COMPOSED_INCLUDED_H
#define VECPP_AP_INT_COMPOSED_INCLUDED_H

#include <cstddef>
#include <cstdint>
#include <climits>
#include <type_traits>

namespace vecpp {
  // This is really simple. An AP int 
  template<std::size_t bits>
  struct Composed_ap_int_impl {
    static_assert(bits > 64);
  };
}

#endif