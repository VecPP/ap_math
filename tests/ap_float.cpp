#include "catch.hpp"

#include "vecpp/ap_math.h"

using float10_10_t = vecpp::Ap_float<10,10>;


TEST_CASE("construct ap float", "[apfloat]") {
  float10_10_t x{3.2};
  (void)x;
}
