#include <cstdio>
#include <vector>

struct S
{
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
    vec.push_back(S());
    /*
     * One object is created (S()), it is moved constructed into its resulting
     * place in the vector (S(&&)), and then both are destructed (~S()) from
     * the vector and the original one.
     */
}
