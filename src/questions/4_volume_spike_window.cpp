// PROBLEM: Trade Volume Spike Window
//
// You're given an array of daily trade volumes and a window size K.
// Find the maximum total volume traded across any K consecutive days.
//
// Example: volumes = [3, 1, 4, 1, 5, 9, 2, 6], K = 3
//   windows: [3,1,4]=8 [1,4,1]=6 [4,1,5]=10 [1,5,9]=15 [5,9,2]=16 [9,2,6]=17
//   -> max = 17
//
// Fill in maxVolumeWindow() below. Target: O(n) time.

#include <iostream>
#include <vector>
#include <numeric>
#include "helpers/helpers.hpp"
using namespace std;

long long maxVolumeWindow(const vector<int>& volumes, int k) {
    // TODO: implement fixed-size sliding window
    // assume k <= volumes.size() - 1;
    auto window_start = 0;
    auto max_volume = 0;
    auto total_size = static_cast<int>(volumes.size());
    for (int window_end = k; window_end <= total_size; window_end++) {
        auto volume_window = vector<int>(volumes.begin() + window_start, volumes.begin() + window_end);
        auto window_sum = std::accumulate(volume_window.begin(), volume_window.end(), 0);
        // dbg(window_start, window_end, vecToString(volume_window), max_volume);
        if (window_sum > max_volume) {
            max_volume = window_sum;
        }
        window_start++;
    }
    return max_volume;
}

void check(const vector<int>& volumes, int k, long long expected) {
    long long result = maxVolumeWindow(volumes, k);
    cout << (result == expected ? "PASS" : "FAIL")
         << " — got " << result << ", expected " << expected << "\n";
}

int main() {
    check({3, 1, 4, 1, 5, 9, 2, 6}, 3, 17);
    check({1, 2, 3, 4, 5}, 2, 9);
    check({5, 5, 5, 5}, 4, 20);
    check({10}, 1, 10);
    check({1, 1, 1, 1, 1, 1}, 3, 3);
    return 0;
}
