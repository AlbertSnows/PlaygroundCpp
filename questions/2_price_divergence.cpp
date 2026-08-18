// PROBLEM: Price Divergence Alert
//
// You're given two arrays of equal length representing the daily closing
// price of Stock A and Stock B over the same period, plus a threshold.
// Return a vector of every day-index where the absolute price difference
// between the two stocks exceeds the threshold.
//
// Example:
//   stockA = [100, 102, 105, 101, 98]
//   stockB = [101, 100, 99,  108, 97]
//   threshold = 5
//   -> difference per day: 1, 2, 6, 7, 1
//   -> indices where diff > 5: [2, 3]
//
// This mirrors the "notify when two stocks have achieved a price
// difference greater than a given threshold" style question.
//
// Fill in findDivergences() below. Target: O(n) time.

#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

vector<int> findDivergences(const vector<int>& stockA, const vector<int>& stockB, int threshold) {
    // TODO: implement
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

void check(const vector<int>& a, const vector<int>& b, int threshold, const vector<int>& expected) {
    vector<int> result = findDivergences(a, b, threshold);
    bool pass = (result == expected);
    cout << (pass ? "PASS" : "FAIL") << " — got ";
    printVec(result);
    cout << ", expected ";
    printVec(expected);
    cout << "\n";
}

int main() {
    check({100, 102, 105, 101, 98}, {101, 100, 99, 108, 97}, 5, {2, 3});
    check({10, 20, 30}, {10, 20, 30}, 0, {});
    check({10, 20, 30}, {5, 25, 50}, 10, {2});
    return 0;
}
