cmake_minimum_required(VERSION 4.1)
project(PlaygroundCpp)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(PlaygroundCpp
        src/questions/1_max_gain_streak.cpp
        src/questions/2_price_divergence.cpp
        src/questions/3_largest_palindrome_product.cpp
        src/helpers/helpers.hpp
)
