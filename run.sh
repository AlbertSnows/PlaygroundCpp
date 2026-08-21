#!/bin/bash
# Edit FILE below to point at whichever problem you're working on.
#FILE=src/questions/1_max_gain_streak.cpp
#FILE=src/questions/2_price_divergence.cpp
#FILE=src/questions/3_largest_palindrome_product.cpp
#FILE=src/questions/4_volume_spike_window.cpp
#FILE=src/questions/5_duplicate_order_id.cpp
#FILE=src/questions/6_best_bid_tracker.cpp
#FILE=src/questions/7_cheapest_active_bid.cpp
FILE=src/Projects/mini_project/main.cpp
g++ -std=c++17 -g -O0 -fno-omit-frame-pointer \
    -Wall -Wextra -Isrc "$FILE" -o run || exit 1

./run
status=$?

if [ "$status" -gt 128 ]; then
    sig=$((status - 128))
    echo "--- crashed with signal $sig, backtrace ---"
    for _ in 1 2 3 4 5 6 7 8 9 10; do
        coredumpctl info -1 >/dev/null 2>&1 && break
        sleep 0.2
    done
    coredumpctl debug -q -1 \
        --debugger-arguments="-q -batch -iex 'set debuginfod enabled off' -ex 'bt full'" \
        2>&1 | grep -v '^warning: Can.t open file'
fi

exit $status
