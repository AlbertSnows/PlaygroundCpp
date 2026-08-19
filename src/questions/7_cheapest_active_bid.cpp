// PROBLEM: Cheapest Active Bid
//
// You're given a sequence of operations on a single stock's resting bids:
//   POST <price>  -- add a new resting bid at that price
//   BUY           -- execute against and remove the single cheapest
//                     currently active bid (no-op if none are active)
// After each operation, report the current cheapest active bid price, or
// -1 if no bids are active.
//
// Example: POST 105, POST 100, POST 110, BUY, POST 98, BUY, BUY
//   -> [105, 100, 100, 105, 98, 105, 110]
//   (BUY removes 100 -> cheapest becomes 105; BUY removes 98 -> cheapest
//    becomes 105 again; BUY removes 105 -> cheapest becomes 110)
//
// Fill in cheapestAfterEachOp() below. Target: O(log n) per
// operation.

#include <iostream>
#include <vector>
#include <string>
#include <utility>
using namespace std;

// op is "POST" or "BUY"; price is meaningless for "BUY" (pass anything).
vector<int> cheapestAfterEachOp(const vector<pair<string, int>>& ops) {
    // TODO: implement using a min-heap (priority_queue<int, vector<int>, greater<int>>)
    return {};
}

void printVec(const vector<int>& v) {
    cout << "[";
    for (size_t i = 0; i < v.size(); i++) {
        cout << v[i];
        if (i + 1 < v.size()) cout << ", ";
    }
    cout << "]";
}

void check(const vector<pair<string, int>>& ops, const vector<int>& expected) {
    auto result = cheapestAfterEachOp(ops);
    bool pass = (result == expected);
    cout << (pass ? "PASS" : "FAIL") << " — got ";
    printVec(result);
    cout << ", expected ";
    printVec(expected);
    cout << "\n";
}

int main() {
    check({{"POST",105}, {"POST",100}, {"POST",110}, {"BUY",0}, {"POST",98}, {"BUY",0}, {"BUY",0}},
          {105, 100, 100, 105, 98, 105, 110});
    check({{"BUY",0}, {"POST",50}, {"POST",20}, {"BUY",0}}, {-1, 50, 20, 50});
    return 0;
}
