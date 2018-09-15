#include "catch.hpp"

#include "vecpp/ap_math/ap_int.h"
#include "vecpp/ap_math/limits.h"

using Int4_t = vecpp::Ap_int<4>;

static_assert(std::numeric_limits<Int4_t>::digits == 4);
static_assert(std::numeric_limits<Int4_t>::min() == -8);
static_assert(std::numeric_limits<Int4_t>::max() == 7);

TEST_CASE("construct small ApInt", "[apint]") {
  Int4_t x{3};
  (void)x;
}

TEST_CASE("negate small ApInt", "[apint]") {
  REQUIRE(-Int4_t{3} == Int4_t{-3});
  REQUIRE(+Int4_t{3} == Int4_t{3});
  
  REQUIRE(-Int4_t{-8} == Int4_t{-8});
}
