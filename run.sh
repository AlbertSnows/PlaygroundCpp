#!/bin/bash
# Edit FILE below to point at whichever problem you're working on.
#FILE=questions/1_max_gain_streak.cpp
#FILE=questions/2_price_divergence.cpp
FILE=questions/3_largest_palindrome_product.cpp

g++ -std=c++17 -O2 -Wall -Wextra "$FILE" -o run && ./run
