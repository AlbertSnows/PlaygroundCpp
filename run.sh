#!/bin/bash
# Edit FILE below to point at whichever problem you're working on.
#FILE=src/questions/1_max_gain_streak.cpp
#FILE=src/questions/2_price_divergence.cpp
#FILE=src/questions/3_largest_palindrome_product.cpp
#FILE=src/questions/4_volume_spike_window.cpp
#FILE=src/questions/5_duplicate_order_id.cpp
#FILE=src/questions/6_best_bid_tracker.cpp
FILE=src/questions/7_cheapest_active_bid.cpp

g++ -std=c++17 -O2 -Wall -Wextra -Isrc "$FILE" -o run && ./run
