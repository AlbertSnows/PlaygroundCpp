## Pattern-matching cheat sheet

Roughly ordered by how likely each is to show up in an array/string-heavy
interview like this one:

- **Kadane's algorithm** — signal: "maximum/minimum sum of a *contiguous*
  subarray." Technique: single left-to-right pass, track a running "best
  sum ending here," reset it when continuing stops helping. 

- **Two pointers, converging from opposite ends** — signal: sorted array +
  find a pair matching some target, "container with most water," palindrome
  checks. Technique: one pointer at each end, always move whichever side
  *can't possibly* be part of a better answer.

- **Two pointers / sliding window, same direction** — signal: "longest/
  shortest substring or subarray satisfying some condition" (no repeated
  characters, sum ≤ target, at most K distinct elements). Technique: expand
  a right pointer to grow the window, shrink a left pointer when the
  condition breaks, track the best window size as you go.

- **Fixed-size sliding window** — signal: "max/min sum of a window of
  exactly size K." Technique: maintain a running sum, add the new element
  and drop the oldest as the window slides — O(n) instead of resumming each
  window from scratch.

- **Hash map for O(1) lookup / counting** — signal: "have I seen this value
  before," "two numbers summing to a target," "find duplicates," frequency
  counting. Technique: one pass building/checking a hash map instead of
  nested loops. 

- **Brute force, then prune** — signal: "find the largest/best combination
  satisfying constraints," explicitly told to brute-force first then
  optimize. Technique: generate candidates in the *right order* (e.g.,
  largest-to-smallest) so you can stop early once nothing better remains
  possible. 

- **Binary search on the answer** — signal: "minimize the maximum X" / "find
  the smallest value such that condition Y holds," on some monotonic space
  (not necessarily a sorted array). Technique: binary search over possible
  *answers*, checking feasibility at each midpoint, rather than searching
  over indices.

- **Greedy** — signal: making the locally-best choice at each step, e.g.,
  interval scheduling. The trap: a greedy rule needs an actual proof (or a
  recognized, established pattern) behind it — "seems reasonable" isn't
  enough. A plausible-sounding greedy rule can be provably wrong, exactly
  like the two-pointer trimming approach was.

