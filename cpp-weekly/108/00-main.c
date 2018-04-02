#include <cstdio>

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
    S s;
}
