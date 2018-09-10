//  Copyright 2018 Francois Chabot
//  (francois.chabot.dev@gmail.com)
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#ifndef VECPP_AP_INT_COMPOSED_INCLUDED_H
#define VECPP_AP_INT_COMPOSED_INCLUDED_H

#include <algorithm>
#include <array>
#include <cassert>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <sstream>
#include <string>
#include <type_traits>

namespace vecpp {
// This is really simple. An AP int
template <std::size_t bits>
struct Composed_ap_int_impl {
  static_assert(bits > 64);

  using Word_type = std::uint64_t;
  using Operand_type = std::int64_t;

  static constexpr std::size_t word_bits = sizeof(Word_type) * CHAR_BIT;
  static constexpr std::size_t word_count = 1 + (bits - 1) / word_bits;
  static_assert(word_count >= 2);

  constexpr Composed_ap_int_impl() : v_{0} {};
  constexpr explicit Composed_ap_int_impl(std::int64_t v);
  constexpr Composed_ap_int_impl(const Composed_ap_int_impl&) = default;
  constexpr Composed_ap_int_impl& operator=(const Composed_ap_int_impl&) =
      default;

  constexpr bool operator==(const Composed_ap_int_impl& rhs) const {
    return v_ == rhs.v_;
  }

  constexpr bool operator==(const Word_type& rhs) const {
    return v_[0] == rhs;
  }

  constexpr bool operator!=(const Composed_ap_int_impl& rhs) const {
    return v_ != rhs.v_;
  }

  constexpr bool operator!=(const Word_type& rhs) const {
    return v_[0] != rhs;
  }

  static constexpr std::size_t which_word(std::size_t bit_pos) {
    return bit_pos / word_bits;
  }

  static constexpr Word_type which_bit(std::size_t bit_pos) {
    return bit_pos % word_bits;
  }

  static constexpr Word_type mask_bit(std::size_t bit_pos) {
    return Word_type(1) << which_bit(bit_pos);
  }

  constexpr Word_type get_word(std::size_t bit_pos) const {
    return v_[which_word(bit_pos)];
  }

  constexpr bool get_bit(std::size_t bit_pos) const {
    assert(bit_pos < bits && "Bit position out of bounds!");
    return (mask_bit(bit_pos) & get_word(bit_pos)) != 0;
  }

  constexpr bool is_negative() const { return get_bit(bits - 1); }

  constexpr Composed_ap_int_impl& clear_unused_bits() {
    constexpr std::size_t last_word_bits = ((bits - 1) % word_bits) + 1;
    constexpr Word_type word_max = ~Word_type(0);
    constexpr Word_type mask = word_max >> (word_bits - last_word_bits);

    v_.back() &= mask;
    return *this;
  }

  std::array<Word_type, word_count> v_;
};

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits>::Composed_ap_int_impl(std::int64_t v) {
  if (v < 0) {
    v_[0] = -v;
    for (std::size_t i = 1; i < word_count; ++i) {
      v_[i] = 0;
    }
    *this = -(*this);
  } else {
    v_[0] = v;
    for (std::size_t i = 1; i < word_count; ++i) {
      v_[i] = 0;
    }
  }
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits> operator~(
    const Composed_ap_int_impl<bits>& rhs) {
  Composed_ap_int_impl<bits> result;

  for (std::size_t i = 0; i < bits; ++i) {
    result.v_[i] = ~rhs.v_[i];
  }
  return result.clear_unused_bits();
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits>& operator++(
    Composed_ap_int_impl<bits>& rhs) {
  using Word_type = typename Composed_ap_int_impl<bits>::Word_type;
  rhs += Word_type(1);
  return rhs;
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits> operator-(
    const Composed_ap_int_impl<bits>& rhs) {
  Composed_ap_int_impl<bits> result = ~rhs;
  ++result;
  return result;
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits> operator+(
    const Composed_ap_int_impl<bits>& lhs,
    const Composed_ap_int_impl<bits>& rhs) {
  Composed_ap_int_impl<bits> result = lhs;
  return result += rhs;
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits>& operator+=(
    Composed_ap_int_impl<bits>& lhs,
    typename Composed_ap_int_impl<bits>::Word_type rhs) {
  for (auto& v : lhs.v_) {
    v += rhs;
    if (v >= rhs) {
      break;
    }
    rhs = 1;
  }
  return lhs;
}

template <std::size_t bits>
constexpr typename Composed_ap_int_impl<bits>::Word_type operator%(
    const Composed_ap_int_impl<bits>& lhs,
    typename Composed_ap_int_impl<bits>::Word_type rhs) {
  Composed_ap_int_impl<bits> tmp = lhs;

  // TODO: AHHHHHHHHHHHHHHHHHHHHHHHH
  while (tmp >= rhs) {
    tmp -= rhs;
  }

  return tmp.v_[0];
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits>& operator+=(
    Composed_ap_int_impl<bits>& lhs, const Composed_ap_int_impl<bits>& rhs) {
  using Word_type = typename Composed_ap_int_impl<bits>::Word_type;

  Word_type carry = 0;
  for (std::size_t i = 0; i < lhs.word_count; ++i) {
    Word_type prev = lhs.v_[i];
    if (carry) {
      lhs.v_[i] += rhs.v_[i] + 1;
      carry = (lhs.v_[i] <= prev);
    } else {
      lhs.v_[i] += rhs.v_[i];
      carry = (lhs.v_[i] < prev);
    }
  }

  return lhs.clear_unused_bits();
}

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits>& operator-=(
    Composed_ap_int_impl<bits>& lhs,
    typename Composed_ap_int_impl<bits>::Word_type rhs) {
  for (auto& v : lhs.v_) {
    auto tmp = v;
    v -= rhs;
    if (rhs <= v) {
      break;
    }
    rhs = 1;
  }
  return lhs;
}

template <std::size_t bits>
constexpr bool operator>(const Composed_ap_int_impl<bits>& lhs,
                         typename Composed_ap_int_impl<bits>::Word_type rhs) {
  if (lhs.is_negative()) {
    return false;
  }

  if (lhs.v_[0] > rhs) {
    return true;
  }

  for (std::size_t i = 1; i < lhs.word_count; ++i) {
    if (lhs.v_[i] != 0) {
      return true;
    }
  }

  return false;
}

template <std::size_t bits>
constexpr bool operator>=(const Composed_ap_int_impl<bits>& lhs,
                          typename Composed_ap_int_impl<bits>::Word_type rhs) {
  if (lhs.is_negative()) {
    return false;
  }

  if (lhs.v_[0] >= rhs) {
    return true;
  }

  for (std::size_t i = 1; i < lhs.word_count; ++i) {
    if (lhs.v_[i] != 0) {
      return true;
    }
  }

  return false;
}


template <std::size_t bits>
constexpr std::tuple<Composed_ap_int_impl<bits>, Composed_ap_int_impl<bits>> 
  udivmod(const Composed_ap_int_impl<bits>& num, const Composed_ap_int_impl<bits>& denum) {
    if(denum > num) {
      return 0;
    }

    auto remainder = num;
    auto divisor = denum;
    std::size_t shift = 0;

    //TODO: just count leading zeros on both numbers...
    while(divisor < num) {
      divisor <<= 1;
      ++shift;
    }

    divisor >>= 1;
    --shift;


    while(1) {
      if(remainder < denum) {
        break;
      }

      if(divisor < remainder) {
        remainder -= divisor;
        result.set_bit(shift);
      }

      divisor <<= 1;

    }
    Composed_ap_int_impl<bits> result{0};

  }

template <std::size_t bits>
constexpr Composed_ap_int_impl<bits>& operator/=(
    Composed_ap_int_impl<bits>& lhs,
    typename Composed_ap_int_impl<bits>::Word_type rhs) {

  return lhs;
}

template <std::size_t bits>
std::ostream& operator<<(std::ostream& stream,
                         const Composed_ap_int_impl<bits>& val) {
  if (val == 0) {
    return stream << "0";
  }

  if (val.is_negative()) {
    return stream << "-" << -val;
  }

  std::ostringstream result;
  auto tmp = val;
  while (tmp != 0) {
    auto offset = int(tmp % 10);
    result << char('0' + int(tmp % 10));
    tmp /= 10;
  }

  std::string result_string = result.str();
  std::reverse(result_string.begin(), result_string.end());
  stream << result_string;
  return stream;
}
}

#endif