# Wolverine Trading / Coderbyte C++ practice

Problem files live under `questions/`. Commands below assume you're running
from the `wolverine/` root.

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

