#include "catch.hpp"

#include "vecpp/ap_math.h"

using UInt80_t = vecpp::Ap_uint<80>;

TEST_CASE("construct ApuInt", "[apuint]") {
  UInt80_t x{24};
  (void)x;
}

TEST_CASE("construct ApuInt from string", "[apuint]") {
  REQUIRE(UInt80_t("1234") == UInt80_t{1234});
  REQUIRE(UInt80_t("00123") == UInt80_t{123});

//  constexpr UInt80_t x("5678");
  //(void)x;
}

TEST_CASE("compare ApuInt", "[apuint]") {
  UInt80_t x{25};
  UInt80_t y{25};
  UInt80_t z{25 + 25};

  REQUIRE(x == y);
  REQUIRE(x != z);
}


TEST_CASE("apuint unary ~", "[apuint]") {
  UInt80_t x{0};

  REQUIRE(~x == std::numeric_limits<UInt80_t>::max());
}

TEST_CASE("apuint comparisons", "[apuint]") {
  UInt80_t a{0};
  UInt80_t b{1};
  UInt80_t c{std::numeric_limits<UInt80_t>::max()};

  REQUIRE(a < b);
  REQUIRE(a < c);

  REQUIRE(b > a);
  REQUIRE(b < c);

  REQUIRE(c > a);
  REQUIRE(c > b);
}

TEST_CASE("apuint ++", "[apuint]") {
  UInt80_t z{0};
  UInt80_t o{1};
  UInt80_t x{0};

  REQUIRE(++x == o);
  REQUIRE(x == o);

  x = z;
  REQUIRE(x++ == z);
  REQUIRE(x == o);
}

TEST_CASE("apuint --", "[apuint]") {
  UInt80_t z{0};
  UInt80_t o{1};
  UInt80_t x{1};

  REQUIRE(--x == z);
  REQUIRE(x == z);

  x = o;
  REQUIRE(x-- == o);
  REQUIRE(x == z);
}

TEST_CASE("ostream << apuint ", "[apuint]") {

  {
    std::ostringstream stream;
    UInt80_t data{"92233720368547758071"};
    stream << data;
    REQUIRE(stream.str() == "92233720368547758071");
  }

}
