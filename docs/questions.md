## Conceptual questions to be ready to explain out loud
These come up directly in the live interview round, not as coding problems:

- **Pointers vs. references** — a pointer can be reassigned and can be null;
  a reference must be bound at declaration and can never be null or rebound.
  Pointers need `*`/`&` to dereference/take address; references act like an
  alias to the original variable.
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
