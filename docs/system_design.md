## System design questions 

--- Systems design questions ---

1. **Order book design** — "Create an in-memory order book supporting
   `add()`, `cancel()` and `get_top_of_book()` with O(log N) per
   operation." Directly matches `src/mini_project/order_book.hpp` already
   in this repo — this is the same problem, not just a related one.
2. **Real-time risk service** — design a service consuming order flow,
   computing P&L and risk exposures, alerting on limit breaches, with a
   1ms latency budget. Key considerations: streaming aggregation of
   per-account/per-instrument state in memory, avoiding lock contention
   and allocation in the hot path (the 1ms budget rules out anything
   GC-like or lock-heavy).
3. **Market data normalizer** — consume raw exchange feeds, produce a
   normalized internal feed. Key considerations: one adapter/parser per
   exchange's wire format, a common internal schema everything downstream
   consumes, handling out-of-order or duplicate messages and detecting
   sequence-number gaps.
4. **Time series database** — store/query market data (trades, quotes,
   snapshots), read-heavy workload. Key considerations: column-oriented
   storage, time-based partitioning, indexing by symbol+timestamp,
   retention/downsampling policy for old data.
5. **Distributed configuration** — share config across 100 trading
   processes, with update distribution. Key considerations: push (pub/sub
   over a message bus) vs. poll, consistency-vs-availability tradeoff if a
   config update fails to reach some processes, versioning/rollback.
6. **Trade reconciliation** — reconcile internal order-entry logs against
   exchange execution reports. Key considerations: what the matching key
   is (order id / exec id), handling partial fills, timing skew between
   the two logs, how discrepancies get surfaced/alerted.
7. **Multi-exchange smart order router** — split a large order across
   fragmented liquidity venues. Key considerations: needs a real-time view
   of liquidity across venues, minimizing market impact/slippage,
   per-venue latency and fee differences. Same domain as the VWAP
   reference from `docs/mini_project_research.md`.

--- Real engineering scenarios (verified, debugging/judgment style, not design-from-scratch) ---

- **Debugging a latency spike** — investigate periodic 5ms spikes when
  steady-state is 50µs.
- **Memory leak in production** — RSS growing 100MB/day until OOM-kill.
- **Thread pinning** — why do trading systems pin threads to specific CPU
  cores? (Answer shape: avoids context-switch overhead and cache
  invalidation from the OS scheduler moving a hot thread between cores.)
- **"Why is my code slow?"** — explain a 30M vs. 100M iterations/second
  performance difference.
- **Pre-mortem** — checklist before deploying a new market-making
  strategy live.

--- Reported topics, not yet independently verified (⚠ search-summary only) ---

- Implementing a function to detect arbitrage opportunities.
- Concurrency — "how do you handle it in your code."
- General framing from multiple sources: "designing a low-latency trading
  system" as a broad topic area, with explicit emphasis on defining
  throughput/latency/reliability requirements before picking
  technologies/architecture.

Sources:
- https://www.quantt.co.uk/resources/quant-developer-interview-questions (verified direct read)
- ⚠ https://www.indeed.com/cmp/Wolverine-Trading/interviews?fjobtitle=Software+Engineer
- ⚠ https://www.glassdoor.com/Interview/Wolverine-Trading-Software-Engineer-Interview-Questions-EI_IE269559.0,17_KO18,35.htm
- ⚠ https://dataford.io/interview-guides/wolverine-trading/software-engineer
