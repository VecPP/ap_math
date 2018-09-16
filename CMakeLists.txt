cmake_minimum_required(VERSION 3.9)

project(ap_math 
  VERSION 0.1
  DESCRIPTION "C++ constexpr math"
  LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

option(AP_MATH_BUILD_TESTS "Build tests" ON)

add_library(ap_math INTERFACE)
add_library(ap_math::ap_math ALIAS ap_math)

target_include_directories(ap_math INTERFACE 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_compile_features(ap_math INTERFACE cxx_std_17)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/vecpp DESTINATION include)

if(AP_MATH_BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()