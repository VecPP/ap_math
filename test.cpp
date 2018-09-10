#include "ap_int.h"

#include <iostream>

int main() {

  vecpp::Ap_int<120> a{0xFFFFFFFF};
  vecpp::Ap_int<120> b{0xFFFFFFFF};

  for(int i = 0 ; i < 20; ++i) {
    a += b;
    std::cout << a << "\n";
  }

  return 0;
}