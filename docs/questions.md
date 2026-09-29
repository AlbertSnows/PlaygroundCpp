## Conceptual questions to be ready to explain out loud
These come up directly in the live interview round, not as coding problems:

- **Pointers vs. references** — a pointer can be reassigned and can be null;
  a reference must be bound at declaration and can never be null or rebound.
  Pointers need `*`/`&` to dereference/take address; references act like an
  alias to the original variable.
  - suppose x = 5
    - x is the local name of th e memory address, 5 is the value in the slot
  - suppose int& ref = x
    - then ref = 10;
  - T& x = Y, is a reference to y.


- **Class vs. object** — a class is the blueprint/type definition; an object
  is a concrete instance of that type allocated in memory.
- **Hash table vs. linked list** — hash table: O(1) average lookup/insert,
  no ordering, needs a good hash function and handles collisions (chaining
  or open addressing). Linked list: O(n) lookup, O(1) insert/delete at a
  known position, preserves insertion order, no hashing overhead.
- **Expected value / probability brain teasers** — practice basic dice-roll
  and card-draw expected value problems; these show up in the live round.
  - *Expected number of flips of a fair coin until the first heads?* — 2.
    Geometric distribution, E = 1/p = 1/0.5.
  - *Expected sum of two fair six-sided dice?* — 7. Linearity of
    expectation: E[die] = 3.5 each, sum of independent expectations adds
    even though the individual rolls aren't independent of the total.
  - *Expected number of cards drawn from a shuffled 52-card deck until the
    first ace appears?* — 53/5 = 10.6. Symmetry trick: the 4 aces split
    the deck into 5 gaps; the 48 non-ace cards distribute evenly across
    those gaps (48/5 each), so the first ace lands after 48/5 + 1 = 10.6
    cards on average. (This is the "slightly advanced" style Wolverine
    candidates report — see docs/mini_project_research.md.)

## Finance-adjacent (option theory)
Some candidates specifically reported option theory coming up — distinct
from the CS/coding/probability content above:

- **Call vs. put** — a call is the *right, not obligation*, to *buy* the
  underlying at a fixed strike price by/at expiration; a put is the right
  to *sell* at the strike. The buyer pays a premium for that right; the
  writer (seller) collects the premium and takes on the obligation to
  fulfill the trade if the buyer exercises.
- **In the money / out of the money / at the money** — for a call: ITM
  means current price > strike (exercising would be profitable, before
  accounting for the premium already paid), OTM means current price <
  strike, ATM means current price ≈ strike. For a put, ITM/OTM flip: ITM
  means current price < strike.
- **Intrinsic value vs. time value** — an option's premium splits into
  intrinsic value (what it'd be worth if exercised right now — how far
  ITM it is, floored at 0) plus time value (extra value reflecting the
  chance it becomes more valuable before expiration). Time value decays
  toward 0 as expiration approaches — "theta decay."
- **Black-Scholes, conceptually** (not the derivation) — prices European-
  style options from five inputs: current price, strike, time to
  expiration, risk-free rate, and volatility. Two intuitions worth having
  ready: higher volatility → higher option price (more uncertainty = more
  chance of a large favorable move), and more time to expiration → higher
  price (more time value). Assumes the underlying follows a lognormal
  random walk (geometric Brownian motion) with no arbitrage opportunities.
- **The Greeks** — sensitivities of an option's price to one underlying
  input at a time (partial derivatives, informally):
  - *Delta* — change in option price per $1 move in the underlying. Calls:
    0 to 1; puts: -1 to 0. Roughly tracks probability of expiring ITM, and
    doubles as a hedge ratio (delta-hedging: hold delta shares of the
    underlying per option to offset small price moves).
  - *Gamma* — change in delta per $1 move in the underlying (delta's own
    sensitivity). Highest for at-the-money options near expiration —
    delta swings fastest there.
  - *Theta* — change in option price per day of time passing, holding
    everything else fixed. Usually negative for a held (long) option —
    this is "theta decay," the time value bleeding away as expiration
    nears.
  - *Vega* — change in option price per 1-point move in implied
    volatility. Higher for longer-dated options (more time for
    volatility to matter).
  - *Rho* — change in option price per 1% move in the risk-free rate;
    usually the least emphasized Greek in interviews.
- **Put-call parity** — a no-arbitrage relationship linking a call and put
  at the *same* strike and expiration on the same underlying:
  `Call - Put = Underlying Price - Strike (discounted to present value)`.
  Intuition: holding a call + cash equal to the discounted strike produces
  the same payoff at expiration as holding a put + one share of the
  underlying — both replicate "own the stock, floor the downside at the
  strike." If the equation doesn't hold, there's a risk-free arbitrage
  (buy the cheap side, sell the expensive side). Useful as a sanity check
  and for deriving one option's price from the other.
