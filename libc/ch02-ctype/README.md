# ctype.h - Character Handling

```
if ('A' <= c && c <= 'Z' || 'a' <= c && c <= 'z')
...

if ('0' <= c && c <= '9')
...

if (c == ' ' || c == '\t' || c == '\n')
...
```

became
```
if (isalpha(c))
...

if (isdigit(c))
...

if (isspace(c))
...
```

A typical text-processing program may average three calls on these functions
for every character from the input stream.
The overhead of calling so many functions often dominates the execution time of
the program.
So, we can either avoid these and go back to bulky, error-prone code, or try it
with some macros.

However, one has tobe areful with macros.
We want our macros to expand to compact code and to be reasonably safe.
If a macro is used in too many places, the executable may get too bulky.
And we must be careful how macros expand (i.e., an argument may be expanded to code
that is executed more than once or not at all).
