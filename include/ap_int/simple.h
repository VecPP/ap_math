//  Copyright 2018 Francois Chabot
//  (francois.chabot.dev@gmail.com)
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef VECPP_AP_INT_SIMPLE_INCLUDED_H
#define VECPP_AP_INT_SIMPLE_INCLUDED_H

#include <cstddef>
#include <cstdint>
#include <climits>
#include <type_traits>
#include <iostream>

namespace vecpp {
    // This is really simple. An AP int 
  template<std::size_t bits, typename Storage>
  struct Simple_ap_int_impl {
    static_assert(bits > 1);
    static_assert(bits <= sizeof(Storage) * CHAR_BIT);
    static_assert(std::is_integral_v<Storage>);
    static_assert(std::is_unsigned_v<Storage>);

    static constexpr std::size_t storage_bits = sizeof(Storage) * CHAR_BIT;

    constexpr Simple_ap_int_impl() = default;
    constexpr Simple_ap_int_impl(Storage v) : v_(v) {}
    constexpr Simple_ap_int_impl(const Simple_ap_int_impl&) = default;
    constexpr Simple_ap_int_impl& operator=(const Simple_ap_int_impl&) = default;

    constexpr operator Storage() const {
      return v_;
    }

    constexpr void clearUnusedBits() {
      constexpr Storage storage_max = ~Storage(0);
      constexpr Storage mask = storage_max >> (storage_bits - bits);
      v_ = v_ & mask;
    }

    Storage v_;
  };


  template<std::size_t bits, typename Storage>
  constexpr Simple_ap_int_impl<bits, Storage> operator+(Simple_ap_int_impl<bits, Storage> lhs, Simple_ap_int_impl<bits, Storage> rhs) {
    Simple_ap_int_impl<bits, Storage> result = lhs;
    return result += rhs;
  }

  template<std::size_t bits, typename Storage>
  constexpr Simple_ap_int_impl<bits, Storage>& operator+=(Simple_ap_int_impl<bits, Storage>& lhs, Simple_ap_int_impl<bits, Storage> rhs) {
    lhs.v_ += rhs.v_;
    lhs.clearUnusedBits();
    return lhs;
  }


  template<std::size_t bits, typename Storage>
  std::ostream& operator<<(std::ostream& stream, Simple_ap_int_impl<bits, Storage> val) {
    if constexpr(std::is_same_v<Storage, std::uint8_t> || std::is_same_v<Storage, unsigned char>) {
      return stream << (int)val.v_;
    }
    else {
      return stream << val.v_;
    }

  }
}

#endif