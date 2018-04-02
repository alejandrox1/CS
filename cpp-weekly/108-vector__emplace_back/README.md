# [Undesrtanding `vector::emplace_back`](https://www.youtube.com/watch?v=uwv1uvi1OTU)

Start with a struct that will give us a debug statement everytime one of the
special member functions is called.

```
g++ -std=c++11 -O3 -Wall -Wextra -Wshadow main.cpp 

objdump -d a.out
```


Using `vecotr::emplace_back()` avoids the move (construction) operation by 
directly calling the constructor in place (where it needs to live inside the vector).

## Further Reading
* [Rvalue references and move constructos](https://en.wikipedia.org/wiki/C%2B%2B11#Rvalue_references_and_move_constructors)
    In C++03, and before, temporaries (rvalues) were intended to never be
    modifiable - just as in C - and were considered to be indistinguishable
    from `const T&` types.
    C++11 adds a new non-constant reference type called an rvalue reference,
    `T&&`. This refers to temporaries that are allowed to be modified after
    they are initialized, for the purpose of allowing move semantics.

    A chronic performance problem with C++03 is the costly and unnedeed deep
    copies that can happen implicitly when objects are passed by value.
    In C++11, a move constructor that takes an rvalue reference to a `vector<T>`
    can copy the pointer to the internal C-style array out of the rvalue into
    the new `vector<T>`, and then set the pointer inside the rvalue as null.
    Since the temporary will never again be used, no code will try to access
    the null pointer, and because the pointer is null, its memory is not
    deleted when it goes out of scope.

* [Return value optimization](https://en.wikipedia.org/wiki/Copy_elision#Return_value_optimization)
