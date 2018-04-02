#include <cstdio>
#include <iostream>
#include <vector>

struct S
{
    S(int)                 { puts("S(int)"); }
    S()                    { puts("S()"); }
    S(const S&)            { puts("S(const S &)"); }
    S(S&&)                 { puts("S(S&&)"); }
    S &operator=(const S &){ puts("operator=(const S &)"); return *this; }
    S &operator=(S&&)      { puts("operaotr=(S&&)"); return *this; } 
    ~S()                   { puts("~S()"); }
};

int main()
{
    std::vector<S> vec;
    std::cout<< vec.capacity() << '\n';
    vec.emplace_back(3);

    /*
     * S(const S &) signifies thecopying of objects into a new location due to
     * us overgrowing the capacity of our vector.
     */
    std::cout<< vec.capacity() << "\n\n";
    vec.emplace_back(4);
    
    std::cout<< vec.capacity() << "\n\n";
    vec.emplace_back(5);

    std::cout<< vec.capacity() << "\n\n";
    vec.emplace_back(6);

    std::cout<< vec.capacity() << "\n\n";
}
