# ap_math

simple compile-time abitrary-sized artihmetic types.

## Example:

```cpp
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
```

## Why?

In order to hit precision objectives in [cste_math](https://github.com/VecPP/cste_math), 
we need to be able to compute values of sizes that are not available with the core types.
