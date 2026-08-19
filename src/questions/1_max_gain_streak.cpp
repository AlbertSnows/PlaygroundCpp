// PROBLEM: Max Gain Streak
//
// You're given an array of daily price CHANGES for a stock (not raw prices —
// each element is how much the price moved that day, and can be negative).
// Find the largest possible sum of a contiguous run of days.
//
// Example: [-2, 1, -3, 4, -1, 2, 1, -5, 4]
//   The best run is [4, -1, 2, 1], summing to 6.
// Fill in maxGainStreak() below. Target: O(n) time, O(1) space.

#include <iostream>
#include <vector>
#include <numeric>
#include "helpers/helpers.hpp"
using namespace std;

int maxGainStreak(const vector<int>& changes) {
    int start = 1;
    auto max_sum = changes[0];
    auto end = changes.size() - 1;
    auto running_sum = changes[0];
    while (start <= end) {
        const auto value = changes[start];
        // dbg(start, value, running_sum, max_sum);
        if (value > (running_sum + value)) {
            running_sum = value;
        } else {
            running_sum += value;
        }
        if (running_sum > max_sum) {
            max_sum = running_sum;
        }
        // dbg(start, running_sum, max_sum, value);
        // printf("===\n");
        start++;
    }
    return max_sum;
}


int maxGainStreak_a1(const vector<int>& changes) {
    // TODO: implement Kadane's algorithm
    // -2, 1, -3
    // start at ends, -2, 4
    // remove lowest, 1, 4
    // if larger, record indexes, (1, 8)
    // (4, 4), if same, look at inner indexes
    // remove whichever has more negative inner
    // if ever larger than current run, update current run
    // repeat until left index >= right
    auto left_index = 0;
    auto right_index = changes.size() -1;
    auto total_sum = std::accumulate(changes.begin(), changes.end(), 0);
    auto current_length = right_index;
    auto current_sum = total_sum;
    while (left_index < right_index) {
        const auto left_value = changes[left_index];
        const auto right_value = changes[right_index];
        dbg(left_index,left_value, "|", right_index, right_value, "|", current_sum, total_sum);
        const auto left_is_smaller = left_value < right_value;
        const auto same_value = left_value == right_value;
        auto value_to_sum = 0;
        if (same_value) {
            const auto next_left = changes[left_index + 1];
            const auto next_right = changes[right_index - 1];
            const auto next_left_is_smallest = next_left <= next_right;
            if (next_left_is_smallest) {
                left_index += 1;
            } else { // next right is smallest
                right_index -= 1;
            }
            value_to_sum = left_value;
        } else if (left_is_smaller) {
            left_index += 1;
            value_to_sum = left_value;
        } else { // right is smaller
            right_index -= 1;
            value_to_sum = right_value;
        }
        if ((current_sum - value_to_sum) > total_sum) {
            total_sum = current_sum - value_to_sum;
        }
        current_length -= 1;
        current_sum -= value_to_sum;
    }
    return total_sum;
}

void check(const vector<int>& input, int expected) {
    int result = maxGainStreak(input);
    cout << (result == expected ? "PASS" : "FAIL")
         << " — got " << result << ", expected " << expected << "\n";
}

int main() {
    check({-2, 1, -3, 4, -1, 2, 1, -5, 4}, 6);  // 4 -1 2 1
    check({1, 2, 3, 4}, 10);
    check({-1, -2, -3}, -1);   // all negative: must still pick a non-empty run
    check({5}, 5);
    check({-3, 8, -1, 2, -5, 6}, 10);
    check({6, -3, -4, 8, 3}, 11);
    check({2, 10, -8, -10, -2, 4, -7, -2}, 12);
    check({0, 0, -7, 5, -7, 5, 3, -9}, 8);
    check({2, 1, 7, -6, -7, 9, 5, -6}, 14);

    return 0;
}
