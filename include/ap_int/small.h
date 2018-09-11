//  Copyright 2018 Francois Chabot
//  (francois.chabot.dev@gmail.com)
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef VECPP_AP_INT_SMALL_INCLUDED_H
#define VECPP_AP_INT_SMALL_INCLUDED_H

#include <cstddef>
#include <cstdint>
#include <climits>
#include <type_traits>
#include <iostream>

namespace vecpp {
    // This is really simple. An AP int 
  template<std::size_t bits, typename Storage>
  struct Small_ap_int {
    static_assert(bits > 1);
    static_assert(bits <= sizeof(Storage) * CHAR_BIT);
    static_assert(std::is_integral_v<Storage>);
    static_assert(std::is_unsigned_v<Storage>);

    static constexpr std::size_t storage_bits = sizeof(Storage) * CHAR_BIT;

    constexpr Small_ap_int() = default;
    constexpr Small_ap_int(Storage v) : v_(v) {}
    constexpr Small_ap_int(const Small_ap_int&) = default;
    constexpr Small_ap_int& operator=(const Small_ap_int&) = default;

    constexpr bool operator==(const Small_ap_int& rhs) const {
      return v_ == rhs.v_;
    }

    constexpr bool operator!=(const Small_ap_int& rhs) const {
      return v_ != rhs.v_;
    }

    constexpr bool operator>(const Small_ap_int& rhs) const {
      return v_ > rhs.v_;
    }

    constexpr bool operator<(const Small_ap_int& rhs) const {
      return v_ < rhs.v_;
    }

    constexpr bool operator>=(const Small_ap_int& rhs) const {
      return v_ >= rhs.v_;
    }

    constexpr bool operator<=(const Small_ap_int& rhs) const {
      return v_ <= rhs.v_;
    }

  private:
    Storage v : bits;

    friend std::ostream& operator<<(std::ostream& stream,
                                    Small_ap_int val);
  };


  template<std::size_t bits, typename Storage>
  std::ostream& operator<<(std::ostream& stream,
                           Small_ap_int<bits, Storage> val) {
    if constexpr(std::is_same_v<Storage, std::int8_t> || std::is_same_v<Storage, char>) {
      return stream << (int)val.v_;
    }
    else {
      return stream << val.v_;
    }
  }
}

#endif