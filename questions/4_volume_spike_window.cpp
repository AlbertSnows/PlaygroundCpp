// PROBLEM: Trade Volume Spike Window
//
// You're given an array of daily trade volumes and a window size K.
// Find the maximum total volume traded across any K consecutive days.
//
// Example: volumes = [3, 1, 4, 1, 5, 9, 2, 6], K = 3
//   windows: [3,1,4]=8 [1,4,1]=6 [4,1,5]=10 [1,5,9]=15 [5,9,2]=16 [9,2,6]=17
//   -> max = 17
//
// This is the classic "fixed-size sliding window" pattern -- see
// docs/techniques.md. Fill in maxVolumeWindow() below. Target: O(n) time.

#include <iostream>
#include <vector>
using namespace std;

long long maxVolumeWindow(const vector<int>& volumes, int k) {
    // TODO: implement fixed-size sliding window
    return 0;
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
