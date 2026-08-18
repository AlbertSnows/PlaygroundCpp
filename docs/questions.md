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
