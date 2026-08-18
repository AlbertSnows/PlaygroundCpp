cmake_minimum_required(VERSION 4.1)
project(PlaygroundCpp)

set(CMAKE_CXX_STANDARD 17)

include_directories(wolverine)

add_executable(PlaygroundCpp
        questions/1_max_gain_streak.cpp
        questions/2_price_divergence.cpp
        questions/3_largest_palindrome_product.cpp)
