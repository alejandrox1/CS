# [Undesrtanding `vector::emplace_back`](https://www.youtube.com/watch?v=uwv1uvi1OTU)

Start with a struct that will give us a debug statement everytime one of the
special member functions is called.

```
g++ -std=c++11 -O3 -Wall -Wextra -Wshadow main.cpp 

objdump -d a.out
```


Using `vecotr::emplace_back()` avoids the move (construction) operation by 
directly calling the constructor in place (where it needs to live inside the vector), 
