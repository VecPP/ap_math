#include "ap_int.h"

#include <iostream>

int main() {

  vecpp::Ap_int<3> a = 1;
  vecpp::Ap_int<3> b = 2;
  for(int i = 0 ; i < 20; ++i) {
    a = a + b;
    std::cout << a << "\n";
  }

  return 0;
}