//  Copyright 2018 Francois Chabot
//  (francois.chabot.dev@gmail.com)
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef VECPP_AP_FLOAT_INCLUDED_H
#define VECPP_AP_FLOAT_INCLUDED_H

#include "vecpp/ap_int.h"

namespace vecpp {
  
  // APFloats are always binary
  // APFloats are always signed
  // Ap_float<11, 85 is a IEEE754 half
  // Ap_float<24, 8> is a IEEE754 float
  // Ap_float<53, 11> is a IEEE754 double
  template<std::size_t mantissa_bits, std::size_t exponent_bits>
  class Ap_float {
    static_assert(mantissa_bits > 1);
    static_assert(exponent_bits > 1);
    
  private:
    // The sign of the mantissa is the sign of the value.
    Ap_int<mantissa_bits> mantissa;
    Ap_int<exponent_bits> exponent;
  };
}

#endif