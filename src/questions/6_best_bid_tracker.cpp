// PROBLEM: Best Bid Tracker
//
// Real reported Wolverine question (paraphrased): "keep track of the
// highest bid for all stocks, and report when the highest bid price
// changes after processing market data."
//
// You're given a stream of bid events, each a (symbol, price) pair, in
// the order they arrive. Whenever a symbol's best (highest) bid seen so
// far increases, record an update for that symbol. Bids that don't beat
// the current best for their symbol produce no update. Return every
// update, in the order they occurred.
//
// Example:
//   events = [("AAPL",100), ("MSFT",50), ("AAPL",105), ("AAPL",102), ("MSFT",55)]
//   -> [("AAPL",100), ("MSFT",50), ("AAPL",105), ("MSFT",55)]
//   (AAPL,102 doesn't beat AAPL's existing best of 105, so no update)
//
// Hash map for O(1) per-symbol lookup pattern -- see docs/techniques.md.
// Fill in bestBidUpdates() below. Target: O(n) time.

#include <iostream>
#include <vector>
#include <string>
#include <utility>
using namespace std;

vector<pair<string, int>> bestBidUpdates(const vector<pair<string, int>>& events) {
    // TODO: implement using an unordered_map<string, int> of best-seen
    // price per symbol
    return {};
}

void printUpdates(const vector<pair<string, int>>& v) {
    cout << "[";
    for (size_t i = 0; i < v.size(); i++) {
        cout << "(" << v[i].first << "," << v[i].second << ")";
        if (i + 1 < v.size()) cout << ", ";
    }
    cout << "]";
}

void check(const vector<pair<string, int>>& events, const vector<pair<string, int>>& expected) {
    auto result = bestBidUpdates(events);
    bool pass = (result == expected);
    cout << (pass ? "PASS" : "FAIL") << " — got ";
    printUpdates(result);
    cout << ", expected ";
    printUpdates(expected);
    cout << "\n";
}

int main() {
    check({{"AAPL",100}, {"MSFT",50}, {"AAPL",105}, {"AAPL",102}, {"MSFT",55}},
          {{"AAPL",100}, {"MSFT",50}, {"AAPL",105}, {"MSFT",55}});
    check({{"GOOG",10}}, {{"GOOG",10}});
    check({{"GOOG",10}, {"GOOG",9}, {"GOOG",8}}, {{"GOOG",10}});
    return 0;
}
