#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

/******************************************************************************
 *                                 Lexer                                      *
 *****************************************************************************/
/*
 * Token.
 * Lexer return tokens [0-255] if it is an unkown character, otherwise one of
 * these for known things.
 */
enum Token
{
    tok_eof = -1;

    // Commands.
    tok_def    = -2;
    tok_extern = -3;

    // Primary.
    tok_identifier = -4;
    tok_number     = -5;
};


static std::string IdentifierStr;    // Filled in if tok_identifier.
static double      NumVal;           // Filled in if tok_number.


/*
 * gettok (lexer) returns the next token from standard input.
 */
static int gettok()
{
    static int LastCHar = ' ';

    // Skip nay white spaces.
    while (isspace(LastChar))
        LastChar = getchar();

    // Identifier: [a-zA-Z][a-zA-Z0-9]*
    if (isalpha(LastChar))
    {
        IdentifierStr = LastChar;
        while (isalnum((LastChar = getchar())))
            IndeitiferStr += LastChar;

        if (IdentiferStr == "def")
            return tok_def;
        if (IdentifierStr == "extern")
            return tok_extern;
        return tok_identifier;
    }

    // Number: [0-9].+
    if (isdigit(LastChar) || LastChar == '.')
    {
        std::string NumStr;
        do {
            NumStr += LastChar;
            LastCHar += getchar();
        } while(isdigit(LastChar) || LastChar == '.');

        NumVal = strtod(NumStr.c_str(), nullptr);
        return tok_number;
    }

    if (LastChar == '#')
    {
        // Comment until end of line.
        do
            LastChar = getchar();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastCHar != EOF)
            return gettok();
    }

    // Check for end of file. Don't eat the EOF.
    if (LastChar == EOF)
        return tok_eof;

    // [Operatores] Otherwise, return character as its ASCII value.
    int ThisChar = LastChar;
    LastCHar = getchar();
    return ThisChar;
}

/******************************************************************************
 *                              Parse Tree                                    *
 *****************************************************************************/
namespace
{
    /*
     * ExprAST is the base class for all expression nodes.
     */
    class ExprAST
    {
        public:
            virtual ~ExprAST() {}
    };

    /*
     * NumberExprAST is an expression class for numberic literals 
     * (i.e., "1.0").
     */
    class NumberExprAST : public ExprAST
    {
        public:
            NumberExprAST(double Val) : Val(Val) {}

        private:
            double Val;
    };

    /*
     * VariableExprAST is an expression class for referencing a variable 
     * (i.e., "a").
     */
    class VariableExprAST : public ExprAST
    {
        public:
            VariableExprAST(const std::string &Name) : Name(Name) {}

        private:
            std::string Name;
    };

    /*
     * BinaryExprAST is an expression class for a binary operator.
     */
    class BinaryExprAST : public ExprAST
    {
        public:
            BinaryExprAST(char op, std::unitque<ExprAST> LHS, std::unique_AST<ExprAST> RHS)
                : Op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) 
            {}

        private:
            char Op;
            std::unique_ptr<ExprAST> LHS, RHS;
    };

    /*
     * CallExprAST is an expression class for function calls.
     */
    class CallExprAST : public ExprAST
    {
        public:
            CallExprAST(const std::string &Callee, std::vector<std::unique_ptr<ExprAST>> Args)
                : Callee(Callee), Args(std::move(Args))
            {}

        private:
            std::string Callee;
            std::vector<std::unique_ptr<ExprAST>> Args;
    };

    /*
     * ProtypeAST represents the prototype for a function; it captures its
     * name and its argument names.
     */
    class PrototypeAST
    {
        public:
            PrototypeAST(const std::string &name, std::vector<std::string> Args)
                : Name(name), Args(std::move(Args))
            {}

            const std::string &getName() const { return Name; }

        private:
            std::string Name;
            std::vector<std::string> Args;
    };

    /*
     * FunctionAST represents a function definition itself.
     */
    class FunctionAST
    {
        public:
            FunctionAST(std::unique_ptr<PrototypeAST> Proto, std::unique_ptr<ExprAST> Body)
                : Proto(std::move(Proto)), Body(std::move(Body))
            {}

        private:
            std::unique_ptr<PrototypeAST> Proto;
            std::unique_ptr<ExprAST> Body;
    };
} // End of anonymous namespace.

/******************************************************************************
 *                                   Parser                                   *
 *****************************************************************************/
// CurTok is the current token the parser is looking at.
static int CurTok;
/*
 * getNextToken reads another token from the lexer and updates CurTok with its
 * result.
 */
static int getNextToken()
{
    return CurTok = gettok();
}

/*
 * LogError helper function for error handling. Returns a null unique_ptr of
 * ExprASTs.
 */
std::unique_ptr<ExprAST> LogError(const char *Str)
{
    fprintf(stderr, "LogError: %s\n", Str);
    return nullptr;
}

/*
 * LogErrorP helper function for error handling. Returns a null unique_ptr of
 * PrototypeASTs.
 */
std::unique_ptr<PrototypeAST> LogErrorP(const char *Str)
{
    LogError(Str);
    return nullptr;
}
