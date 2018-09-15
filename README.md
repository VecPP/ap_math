# ap_math

simple compile-time abitrary-sized artihmetic types.

## Ap_int<>

Behaves like native types as much as humanly possible.

- Arithmetic
- `std::numeric_limits<>` specialization.
- `std::ostream` support.

The only real difference with native types is that numbers greater than the largest `uint64_t` cannot be initialized using integre literals, so you'll have to use the string-based constructor (which is constexpr too!).

## Example:

```cpp
#include "vecpp/ap_math/ap_int.h"
#include "vecpp/ap_math/limits.h"
#include <iostream>

int main() {

  vecpp::Ap_int<80> a{"1234567890123456789012345"};
  vecpp::Ap_int<80> b = 12;

  a = a * b;
  std::cout << a << "\n";
   
  // ap_math also support very small ints for completeness
  vecpp::Ap_int<3> a = 1;
  vecpp::Ap_int<3> b = 2;
  for(int i = 0 ; i < 20; ++i) {
    a = a + b;
    std::cout << a << "\n";
  }

  // std::numeric_limits<>
  std::cout << "the largest possible 265 bits int: " << std::numeric_limits<vecpp::Ap_int<265>>::max() << "\n"; 

  return 0;
}
```

## Why?

In order to hit precision objectives in [cste_math](https://github.com/VecPP/cste_math), 
we need to be able to compute values of sizes that are not available with the core types.

## Acknowledgements

The implementation details are heavily inspired from `llvm::APInt` and `llvm::APFloat`.
