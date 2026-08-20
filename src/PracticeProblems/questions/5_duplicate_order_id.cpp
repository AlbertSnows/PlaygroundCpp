// PROBLEM: Duplicate Order ID Detector
//
// You're given a sequence of order IDs as they're submitted throughout the
// day. Return the first order ID that gets submitted twice (scanning left
// to right, the ID value at the point its *second* occurrence appears), or
// -1 if every ID is unique.
//
// Fill in firstDuplicateOrder() below. Target: O(n) time.

#include <iostream>
#include <vector>
#include <unordered_set>
using namespace std;

int firstDuplicateOrder(const vector<int>& ids) {
    unordered_set<int> visited_ids = unordered_set<int>();
    for (int index = 0; index < static_cast<int>(ids.size()); index++) {
        auto current_id = ids[index];
        if (visited_ids.find(current_id) != visited_ids.end()) {
            return current_id;
        }
        visited_ids.insert(current_id);
    }
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
