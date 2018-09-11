#include "catch.hpp"

#include "ap_int.h"

using Int80_t = vecpp::Ap_int<80>;

TEST_CASE("construct ApInt", "[apint]") {
  Int80_t x{25};
  (void)x;
}

TEST_CASE("compare ApInt", "[apint]") {
  vecpp::Ap_int<80> x{25};
  vecpp::Ap_int<80> y{25};
  vecpp::Ap_int<80> z{25 + 25};

  REQUIRE(x == y);
  REQUIRE(x != z);
}

TEST_CASE("apint unary + and -", "[apint]") {
  vecpp::Ap_int<80> x{25};
  vecpp::Ap_int<80> y{-25};

  REQUIRE(+x == x);
  REQUIRE(-x == y);
  REQUIRE(-y == x);
}

TEST_CASE("apint unary ~", "[apint]") {
  vecpp::Ap_int<80> x{0};
  vecpp::Ap_int<80> y{-1};

  REQUIRE(~x == y);
  REQUIRE(~y == x);
}

TEST_CASE("apint ++", "[apint]") {
  vecpp::Ap_int<80> z{0};
  vecpp::Ap_int<80> o{1};
  vecpp::Ap_int<80> x{0};
  vecpp::Ap_int<80> y{-1};

  REQUIRE(++x == o);
  REQUIRE(x == o);

  x = z;
  REQUIRE(x++ == z);
  REQUIRE(x == o);
}

TEST_CASE("apint --", "[apint]") {
  vecpp::Ap_int<80> z{0};
  vecpp::Ap_int<80> o{1};
  vecpp::Ap_int<80> x{1};
  vecpp::Ap_int<80> y{-1};

  REQUIRE(--x == z);
  REQUIRE(x == z);

  x = o;
  REQUIRE(x-- == o);
  REQUIRE(x == z);
}

TEST_CASE("apint comparisons", "[apint]") {
  vecpp::Ap_int<80> a{-2};
  vecpp::Ap_int<80> b{-1};
  vecpp::Ap_int<80> c{0};
  vecpp::Ap_int<80> d{1};
  vecpp::Ap_int<80> e{2};

  REQUIRE(a < b);
  REQUIRE(a < c);
  REQUIRE(a < d);
  REQUIRE(a < e);

  REQUIRE(b > a);
  REQUIRE(b < c);
  REQUIRE(b < d);
  REQUIRE(b < e);

  REQUIRE(c > a);
  REQUIRE(c > b);
  REQUIRE(c < d);
  REQUIRE(c < e);

  REQUIRE(d > a);
  REQUIRE(d > b);
  REQUIRE(d > c);
  REQUIRE(d < e);

  REQUIRE(e > a);
  REQUIRE(e > b);
  REQUIRE(e > c);
  REQUIRE(e > d);
}