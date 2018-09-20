include_directories(.)

add_library(catch_main catch_main.cpp)
set_target_properties(catch_main PROPERTIES FOLDER "tests")

SET( AP_MATH_TESTS
  large_int.cpp
  large_uint.cpp
  small_int.cpp
  ap_float.cpp
)

MACRO(config_test_target TGT)

target_link_libraries(${TGT} catch_main ap_math::ap_math)
set_target_properties(${TGT} PROPERTIES FOLDER "tests")
add_test(${TGT} ${TGT})

if(MSVC)
  target_compile_options(${TGT} PRIVATE /W4 /WX)
else(MSVC)
  target_compile_options(${TGT} PRIVATE -Wall -Wextra -pedantic -Werror)
endif(MSVC)
endmacro()

add_executable(all_tests ${AP_MATH_TESTS})
config_test_target(all_tests)