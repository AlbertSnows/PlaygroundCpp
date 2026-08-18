// PROBLEM: Largest Palindrome Product
//
// A palindromic number reads the same both ways (e.g. 9009).
// Find the largest palindrome made from the product of two n-digit numbers.
//
// Example: n = 2 -> largest palindrome from two 2-digit numbers is
//   91 * 99 = 9009
//
// Candidates report being asked to write a brute-force version first,
// then OPTIMIZE it — so implement both:
//   1. bruteForce(): check every pair, O((10^n)^2)
//   2. optimized(): start from the top of the range and prune early once
//      a product can no longer beat the current best (should be
//      dramatically faster for n=3 or n=4)
//
// Try timing both for n=3 to see the difference.

#include <iostream>
#include <string>
#include <algorithm>
#include <chrono>
using namespace std;

bool isPalindrome(long long num) {
    string s = to_string(num);
    string r = s;
    reverse(r.begin(), r.end());
    return s == r;
}

long long bruteForce(int n) {
    long long low = 1;
    for (int i = 1; i < n; i++) low *= 10;
    long long high = low * 10 - 1;

    long long best = 0;
    // TODO: check every pair (i, j) in [low, high], track largest palindrome product
    return best;
}

long long optimized(int n) {
    long long low = 1;
    for (int i = 1; i < n; i++) low *= 10;
    long long high = low * 10 - 1;

    // TODO: iterate i from high down to low, j from high down to i,
    // break early once i*i <= best (no larger product possible from here)
    return 0;
}

int main() {
    cout << "n=2 brute force: " << bruteForce(2) << " (expected 9009)\n";
    cout << "n=2 optimized:   " << optimized(2) << " (expected 9009)\n";

    auto start = chrono::high_resolution_clock::now();
    long long result = optimized(3);
    auto end = chrono::high_resolution_clock::now();
    cout << "n=3 optimized: " << result << " (expected 906609), took "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms\n";
    return 0;
}
