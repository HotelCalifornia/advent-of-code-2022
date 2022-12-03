cmake_minimum_required(VERSION 3.25.0)

project(AdventOfCode2022 CXX)

add_library(hotel src/common/debug.cpp)
target_compile_features(hotel PUBLIC cxx_std_20)
target_include_directories(hotel PUBLIC include)

add_executable(day01 src/day01/main.cpp)
target_compile_features(day01 PUBLIC cxx_std_20)

add_executable(day02 src/day02/main.cpp)
target_compile_features(day02 PUBLIC cxx_std_20)

add_executable(day03 src/day03/main.cpp)
target_link_libraries(day03 PUBLIC hotel)
