cmake_minimum_required(VERSION 3.25.0)

project(AdventOfCode2022 CXX)

add_executable(day01 src/day01/main.cpp)
target_compile_features(day01 PUBLIC cxx_std_20)

