# [When noexcept really matters](https://www.youtube.com/watch?v=AG_63_edgUg&feature=youtu.be&a=)

```
g++ -std=c++11 -O3 -Wall -Wextra -Wshadow main.cpp 

objdump -d a.out
```

When we grow past the capacity of our vector we see copies being made into the
new location in memory.
However, this leaves us with another question: why copies? Why not moves?

Using `noexcept` we tell the stdlib we have something that can be moved while
maintaining the strong exception guarantee (if an excpetion is thrown, the
operation has no effect). By doing this, we are able to now do moves, 
instead of copies.

# Further Readings
* [Exception safety](https://en.wikipedia.org/wiki/Exception_safety) 
  1. Strong exception safety, also known as commit or rollback semantics:
  Operations can fail, but failed operations are guaranteed to have no side
  effects, so all data retains their original values.

  2. Basic exception safety, also known as a no-leak guarantee: Partial execution
  of failed operations can cause side effects, but all invariants are preserved
  and there are no resource leaks (including memory leaks). Any stored data
  will contain valid values, even if they differ from what they were before the
  exception.
