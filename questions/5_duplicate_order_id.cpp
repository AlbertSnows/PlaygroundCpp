// PROBLEM: Duplicate Order ID Detector
//
// You're given a sequence of order IDs as they're submitted throughout the
// day. Return the first order ID that gets submitted twice (scanning left
// to right, the ID value at the point its *second* occurrence appears), or
// -1 if every ID is unique.
//
// Example: ids = [101, 102, 103, 102, 104]
//   101 new, 102 new, 103 new, 102 -> already seen -> return 102
//
// Classic "hash map for O(1) lookup" pattern -- see docs/techniques.md.
// Fill in firstDuplicateOrder() below. Target: O(n) time.

#include <iostream>
#include <vector>
using namespace std;

int firstDuplicateOrder(const vector<int>& ids) {
    // TODO: implement using a hash set to track seen IDs
    return -1;
}

void check(const vector<int>& ids, int expected) {
    int result = firstDuplicateOrder(ids);
    cout << (result == expected ? "PASS" : "FAIL")
         << " — got " << result << ", expected " << expected << "\n";
}

int main() {
    check({101, 102, 103, 102, 104}, 102);
    check({5, 6, 7, 8}, -1);
    check({1, 1, 2, 3}, 1);
    check({7, 8, 9, 7, 8}, 7);
    check({42}, -1);
    return 0;
}
