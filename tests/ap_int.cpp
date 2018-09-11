#include "catch.hpp"

#include "ap_int.h"

using Int80_t = vecpp::Ap_int<80>;

TEST_CASE("construct ApInt", "[apint]") {
  Int80_t x{25};
  (void)x;
}

TEST_CASE("compare ApInt", "[apint]") {
  Int80_t x{25};
  Int80_t y{25};
  Int80_t z{25 + 25};

  REQUIRE(x == y);
  REQUIRE(x != z);
}

TEST_CASE("apint unary + and -", "[apint]") {
  Int80_t x{25};
  Int80_t y{-25};

  REQUIRE(+x == x);
  REQUIRE(-x == y);
  REQUIRE(-y == x);
}

TEST_CASE("apint unary ~", "[apint]") {
  Int80_t x{0};
  Int80_t y{-1};

  REQUIRE(~x == y);
  REQUIRE(~y == x);
}

TEST_CASE("apint comparisons", "[apint]") {
  Int80_t a{-2};
  Int80_t b{-1};
  Int80_t c{0};
  Int80_t d{1};
  Int80_t e{2};

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

TEST_CASE("apint vs constant", "[apint]") {
  Int80_t a{-2};
  Int80_t b{-1};
  Int80_t c{0};
  Int80_t d{1};
  Int80_t e{2};

  REQUIRE(a == -2);
  REQUIRE(a < -1);
  REQUIRE(a < 0);
  REQUIRE(a < 1);
  REQUIRE(a < 2);

  REQUIRE(b > -2);
  REQUIRE(b == -1);
  REQUIRE(b < 0);
  REQUIRE(b < 1);
  REQUIRE(b < 2);

  REQUIRE(c > -2);
  REQUIRE(c > -1);
  REQUIRE(c == 0);
  REQUIRE(c < 1);
  REQUIRE(c < 2);

  REQUIRE(d > -2);
  REQUIRE(d > -1);
  REQUIRE(d > 0);
  REQUIRE(d == 1);
  REQUIRE(d < e);

  REQUIRE(e > -2);
  REQUIRE(e > -1);
  REQUIRE(e > 0);
  REQUIRE(e > 1);
  REQUIRE(e == 2);
}

TEST_CASE("apint ++", "[apint]") {
  Int80_t z{0};
  Int80_t o{1};
  Int80_t x{0};
  Int80_t y{-1};

  REQUIRE(++x == o);
  REQUIRE(x == o);

  x = z;
  REQUIRE(x++ == z);
  REQUIRE(x == o);
}

TEST_CASE("apint --", "[apint]") {
  Int80_t z{0};
  Int80_t o{1};
  Int80_t x{1};
  Int80_t y{-1};

  REQUIRE(--x == z);
  REQUIRE(x == z);

  x = o;
  REQUIRE(x-- == o);
  REQUIRE(x == z);
}

TEST_CASE("apint += raw", "[apint]") {
  {
    Int80_t x{0};
    x += 5;

    REQUIRE(x == Int80_t(5));
  }

  { 
    Int80_t x{0};
    std::int64_t v = std::numeric_limits<std::int64_t>::min();

    x += v;
    Int80_t expected(v);
    REQUIRE(x == expected);

  }
}

TEST_CASE("apint << raw", "[apint]") {
  {
    REQUIRE(Int80_t{0} << 20 == Int80_t{0});

    REQUIRE(Int80_t{1} << 2 == Int80_t{4});
  }
}

TEST_CASE("apint >> raw", "[apint]") {
  {
    REQUIRE(Int80_t{4} >> 2 == Int80_t{1});

    {
      std::int64_t ref = -200;
      ref >>= 2;

      REQUIRE(Int80_t{-200} >> 2 == Int80_t{ref});
    }
    
    {
      std::int64_t ref = std::numeric_limits<std::int64_t>::min() + 200;
      Int80_t test{ref};
      test <<= 2;
      test >>= 49;

      REQUIRE(test == Int80_t{ref >> 47});
    }

    {
      vecpp::Ap_int<360> x{-500};

      x <<= 250;
      x >>= 250;
      REQUIRE(x == vecpp::Ap_int<360>{-500});
    }
  }
}

TEST_CASE("apint + apint", "[apint]") {
  Int80_t large {std::numeric_limits<std::int64_t>::max() };

    
  REQUIRE((large + large) + (large + large) == large + large + large + large);
}

TEST_CASE("apint * scalar", "[apint]") {
  
  REQUIRE(Int80_t{3} * 4 == Int80_t{12}); 
  REQUIRE(Int80_t{-3} * 4 == Int80_t{-12});
  REQUIRE(Int80_t{3} * -4 == Int80_t{-12});
}
