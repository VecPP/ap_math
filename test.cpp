#include "ap_int.h"

#include <iostream>
#include <iomanip>

int main() {
  int64_t init = 0xFFFFFFFFFFFFFFFF;

  vecpp::Ap_int<66> a{300};
  vecpp::Ap_int<66> b{11};

  auto [res, rem] = udivmod(a, b);
//  std::cout << a.count_leading_zeros() << "\n";
  std::cout << res.v_[0] << " " << rem.v_[0] << "\n";

  return 0;
}