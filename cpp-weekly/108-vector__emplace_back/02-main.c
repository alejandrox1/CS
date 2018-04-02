#include <cstdio>
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

    // Push back a single S obj.
    //vec.emplace_back();
    /*
     * emplace_back has exactly 1 construction and 1 destruction.
     * This is because emplace_back calls the constructor in place.
     */
    vec.emplace_back(3);
}
