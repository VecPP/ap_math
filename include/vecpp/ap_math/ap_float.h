//  Copyright 2018 Francois Chabot
//  (francois.chabot.dev@gmail.com)
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef VECPP_AP_FLOAT_INCLUDED_H
#define VECPP_AP_FLOAT_INCLUDED_H

#include "vecpp/ap_math//ap_int.h"

namespace vecpp {
  
  // APFloats are always binary
  // APFloats are always signed
  // Ap_float<11, 5 is a IEEE754 half
  // Ap_float<24, 8> is a IEEE754 float
  // Ap_float<53, 11> is a IEEE754 double
  template<std::size_t mantissa_bits, std::size_t exponent_bits>
  class Ap_float {
    static_assert(mantissa_bits > 1);
    static_assert(exponent_bits > 1);
    

    constexpr bool operator==(const Ap_float& rhs) const ;
    constexpr bool operator!=(const Ap_float& rhs) const;
    constexpr bool operator<(const Ap_float&) const;
    constexpr bool operator<=(const Ap_float&) const;
    constexpr bool operator>(const Ap_float&) const;
    constexpr bool operator>=(const Ap_float&) const;


  private:

    constexpr int compare(const Ap_float& rhs) const;
    constexpr bool is_negative() const {return m_.is_negative();}

    enum class State {
      normal,
      nan,
      pos_inf,
      neg_inf,
    };

    // The sign of the mantissa is the sign of the value.
    Ap_int<mantissa_bits> m_;
    Ap_int<exponent_bits> e_;
    State s_ = State::normal;
  };

  template<std::size_t M, std::size_t E>
  constexpr int Ap_float<M, E>::compare(const Ap_float& rhs) const {
    switch(s_) {
      case State::pos_inf:
        return rhs.s_ == State::pos_inf ? 0 : 1;
      case State::neg_inf:
        return rhs.s_ == State::neg_inf ? 0 : -1;
      default: 
        break;
    }

    auto l_neg = is_negative();
    auto r_neg = rhs.is_negative();

    if (l_neg != r_neg) {
      return l_neg ? -1 : 1;
    }

    auto e_cmp = e_.compare(rhs.e_);
    if(e_cmp < 0) {
      return l_neg ? 1 : -1;
    }

    if(e_cmp > 0) {
      return l_neg ? -1 : 1;
    }


    return m_.compare(rhs.m_);
  }

  template<std::size_t M, std::size_t E>
  constexpr bool Ap_float<M, E>::operator<(const Ap_float& rhs) const {
    if( s_ == State::nan || rhs.s_ == State::nan) {
      return false;
    }
    return compare(rhs) < 0;
  }

  template<std::size_t M, std::size_t E>
  constexpr bool Ap_float<M, E>::operator<=(const Ap_float& rhs) const {
    if( s_ == State::nan || rhs.s_ == State::nan) {
      return false;
    }
    return compare(rhs) <= 0;
  }

  template<std::size_t M, std::size_t E>
  constexpr bool Ap_float<M, E>::operator>(const Ap_float& rhs) const {
    if( s_ == State::nan || rhs.s_ == State::nan) {
      return false;
    }
    return compare(rhs) > 0;
  }

  template<std::size_t M, std::size_t E>
  constexpr bool Ap_float<M, E>::operator>=(const Ap_float& rhs) const {
    if( s_ == State::nan || rhs.s_ == State::nan) {
      return false;
    }
    return compare(rhs) >= 0;
  }
  template<std::size_t M, std::size_t E>
  constexpr bool Ap_float<M, E>::operator==(const Ap_float& rhs) const { 
    if(rhs.s_ != s_) {
      return false;
    }

    switch(s_) {
      case State::normal:
        return rhs.m_ == rhs.m_ && e_ == rhs.e_;
        break;
      case State::nan:
        return false;
        break;
      case State::pos_inf:
        return rhs.s_ == State::pos_inf;
        break;
      case State::neg_inf:
        return rhs.s_ == State::neg_inf;
        break;
    }
    assert(false);
    return false;
  }

  template<std::size_t M, std::size_t E>
  constexpr bool Ap_float<M, E>::operator!=(const Ap_float& rhs) const {
    return !(*this == rhs);
  }

}

#endif