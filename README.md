# Wolverine Trading / Coderbyte C++ practice

Problem files live under `questions/`. Commands below assume you're running
from this repo's root (`PlaygroundCpp/`).

## Building & running

Compile and run any file with:
```
g++ -std=c++17 -O2 questions/1_max_gain_streak.cpp -o run && ./run
```

## Auto-recompile on save

C++ has no built-in support for watch-on-save (`make`/`cmake`/`ninja` support
incremental rebuilds, but none of them trigger automatically on file
changes). `watchexec` (a general-purpose, language-agnostic file watcher)
fills that gap.

Install (one-time):
```
cargo install watchexec-cli
```

Inline `bash -c '...'` commands turned out unreliable in this shell setup
(quoting kept getting lost between the terminal and watchexec, causing
`g++: fatal error: no input files`). `run.sh` sidesteps that entirely —
watchexec just execs the script file directly, no quoted string involved.
Edit `run.sh`'s `FILE=` line to point at whichever problem you're working
on, then:
```
watchexec -w questions -e cpp -- ./run.sh
```

## CLion setup

`Settings → Build, Execution, Deployment → Toolchains` — CLion should
auto-detect a "System" toolchain from PATH; if not, add one (`+ → System`)
and set:
- **CMake**: `/usr/bin/cmake`
- **Build Tool**: Make or Ninja (both installed)
- **C Compiler**: `/usr/bin/gcc`
- **C++ Compiler**: `/usr/bin/g++`

## Coding problems (`questions/`)
1. `1_max_gain_streak.cpp` — maximum subarray sum (Kadane's algorithm)
2. `2_price_divergence.cpp` — linear scan / threshold comparison across two arrays
3. `3_largest_palindrome_product.cpp` — brute force vs. optimized, with timing
4. `4_volume_spike_window.cpp` — fixed-size sliding window (max sum of any K consecutive days)
5. `5_duplicate_order_id.cpp` — hash-set lookup (first repeated order ID)
6. `6_best_bid_tracker.cpp` — hash map, per-symbol running max (real reported Wolverine question)
7. `7_cheapest_active_bid.cpp` — min-heap, insert + remove-min (real reported Wolverine question)

## Mini project (`mini_project/`)

A small in-memory limit order book — see `docs/mini_project_research.md`
for why, and the header comment in `mini_project/order_book.hpp` for the
exact contract. Multi-file, not covered by `run.sh`; compile directly:
```
g++ -std=c++17 -O2 mini_project/main.cpp mini_project/order_book.cpp -o run && ./run
```

## Other docs
- `docs/mini_project_research.md` — research on Wolverine's actual assessment format and trading-firm mini-project themes
- `docs/questions.md` — conceptual questions for the live round (pointers/references, class/object, hash table/linked list, expected-value brainteasers)
- `docs/techniques.md` — pattern-matching cheat sheet for coding problems

