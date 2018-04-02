#include <cstdio>
#include <iostream>
#include <vector>

struct S
{
    S(int) noexcept                  { puts("S(int)"); }
    S() noexcept                     { puts("S()"); }
    S(const S&) noexcept             { puts("S(const S &)"); }
    S(S&&) noexcept                  { puts("S(S&&)"); }
    S &operator=(const S &) noexcept { puts("operator=(const S &)"); return *this; }
    S &operator=(S&&) noexcept       { puts("operaotr=(S&&)"); return *this; } 
    ~S() noexcept                    { puts("~S()"); }
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
