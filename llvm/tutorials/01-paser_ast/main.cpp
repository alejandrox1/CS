#include "llvm/ADT/STLExras.h"
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
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
 * BinopPrecedence - This holds the precedence for each binary operator that is
 * defined.
 */
static std::map<char, int> BinoPrecedence;

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

/*
 * GetTokPrecedence - Get the precedence of the pending binary token.
 */
static int GetTokPrecendece()
{
    if (!isascii(CurTok))
        return -1;

    // Make sure it's a declared binop.
    int TocPrec = BinoPrecedence[CurTok];
    if (TocPrec <= 0) 
        return -1;

    return TocPrek;
}

/*
 * ParseNumberExpr parses a numeric literal.
 */
static std::unique_ptr<ExprAST> ParseNumberExpr()
{
    auto Result = llvm::make_unique<NumberExprAST>(NumVal);
    getNextToken(); // get the number.
    return std::move(Result);
}

/*
 * ParseParenExpr parses an expression between parenthesis.
 */
static std::unique_ptr<ExprAST> ParseParenExpr()
{
    getNextToken(); // get the '('.
    auto V = ParseExpression();
    if (!V)
        return nullptr;

    if (CurTok != ')')
        return LogError("Expected ')'");

    getNextToken(); // get ')'.
    return V;
}

/*
 * ParseIdentifierExpr parses variable references and function calls.
 */
static std::unique_ptr<ExprAST> ParseIdentifierExpr()
{
    std::string IdName = IdentiferStr;

    getNextToken(); // get identfier.

    // Simple variable reference.
    if (CurTok != '(')
        return llvm::make_unique<VariableExprAST>(IdName);

    // Call.
    getNextToken();  // get '('.
    std::vector<std::unique_ptr<ExprAST>> Args;
    if (CurTok != ')')
    {
        // Parse arguments list.
        while (true)
        {
            if (auto Arg = ParseExpression())
                Args.push_back(std::move(Arg));
            else
                nullptr;

            if (CurTok == ')')
                break;

            if (CurTok != ',')
                return LogError("Expected ')' or ',' in argument list.");

            getNextToken();
        }
    }

    getNextToken();  // get ')'.

    return llvm::make_unique<CallExprAST> (IdName, std::move(Args));
}

/*
 * ParsePrimary parses IdentiferExpr, NumberExpr, and ParenExpr.
 */
static std::unique_ptr<ExprAST> ParsePrimary()
{
    switch (CurTok)
    {
        default:
            return LogError("Unkown token when expecting an expression.");
        case tok_identifier:
            return ParseIdentifierExpr();
        case tok_number:
            return ParseNumberExpr();
        case '(':
            return ParseParenExpr();
    }
}

/*
 * ParseExpression 
 */
static std::unique_ptr<ExprAST> ParseExpression()
{
    auto LHS = ParsePrimary();
    if (!LHS)
        return nullptr;

    return ParseBinOpRHS(0, std::move(LHS));
}

/*
 * ParseBinOpRHS - 
 */
static std::unique_ptr<ExprAST> ParseBinOpRHS(int ExprPrec, std::unique_ptr<ExprAST> LHS)
{
    // If this is a binop, find its precedence.
    while (true)
    {
        if TokPrec = GetTokPrecedence();

        // If this is a binop that binds at least as tightly as the current
        // binop, consume it, otherwise, we are done.
        if (TokPrec < ExprPrec)
            return LHS;

        // We now this is a binop.
        int BinOp = CurTok;
        getNextToken();    // Consume the binop.

        // Parse primary expression after binary operator.
        auto RHS = ParsePrimary();
        if (!RHS)
            return nullptr;

        // If BinOp binds less tightly with RHS than the operator after RHS,
        // let the pending operator take RHS as its LHS.
        int NextPrec = GetTokenPrecedence();
        if (TokPrec < NextPrec)
        {
            RHS = ParseBinOpRHS(TokPrec + 1, std::move(RHS));
            if (!RHS)
                return nullptr;
        }

        // Merge LHS/RHS.
        LHS = llvm::make_unique<BinaryExprAST>(BinOp, std::move(LHS), std::move(RHS));
    }
}

/*
 * ParsePrototype - 
 */
static std::unique_ptr<ProtypeAST> ParsePrototype()
{
    if (CurTok != tok_identifier)
        return LogErrorP("Expected function name in prototype");
    
    std::string FnName = IdentifierStr;
    getNextToken();

    if (CurTok != '(')
        return LogErrorP("Expected '(' in prototype");

    // Read the list of arguments.
    std::vector<std::string> ArgNames;
    while (getNextToken() == tok_identifier)
        ArgNames.push_back(IdentifierStr);

    if (CurTok != ')')
        return LogErrorP("Expected ')' in prototype");

    // Success.
    getNextToken();   // Eat '('.

    return llvm::make_unique<PrototypeAST>(FnName, std::move(ArgNames));
}

/*
 * ParseDefinition - 
 */
static std::unique_ptr<FunctionAST> ParseDefinition()
{
    getNextToken();    // eat def.
    auto Proto = ParsePrototype();
    if (!Proto)
        return nullptr;

    if (auto E = ParseExpression())
        return llvm::make_unique<FunctionAST>(std::move(Proto), std::move(E));
 
    return nullptr;
}

/*
 * ParseExtern -
 */
static std::unique_ptr<PrototypeASP> ParseExtern()
{
    getNextToken();    // eat extern.
    return ParsePrototype();
}

/*
 * ParseTopLevelExpr - 
 */
static std::unique_ptr<FunctionAST> ParseTopLevelExpr()
{
    if (auto E = ParseExpression())
    {
        // Make an annonymous proto.
        auto Proto = llvm::make_unique<PrototypeAST>("", std::vector<std::string>());
        return llvm::make_unique<FunctionAST>(std::move(Proto), std::move(E));
    }
    return nullptr;
}

/******************************************************************************
 *                              Top-Level Parsing                             *
 *****************************************************************************/
static void HandleDefinition()
{
    if (ParseDefinition()) {
        fprintf(stderr, "Parsed a function definition.\n");
    } else {
        // Skip token for error recovery.
        getNextToken();
    }
}

static void HandleExtern()
{
    if (ParseExtern()) {
        fprintf(stderr, "Parsed an extern\n");
    } else {
        // Skip token for error recovery.
        getNextToken();
    }
}

static void HandleTopLevelExpression()
{
    // Evaluate a top-level expression into an anonymous function.
    if (ParseTopLevelExpr()) {
        fprintf(stderr, "Parsed a top-level expr\n");
    } else {
        // Skip token for error recovery.
        getNextToken();
    }
}

/// top ::= definition | external | expression | ';'
static void MainLoop()
{
    while (true)
    {
        fprintf(stderr, "ready> ");
        switch (CurTok)
        {
            case tok_eof:
                return;
            case ';': // ignore top-level semicolons.
                getNextToken();
                break;
            case tok_def:
                HandleDefinition();
                break;
            case tok_extern:
                HandleExtern();
                break;
            default:
                HandleTopLevelExpression();
                break;
        }
    }
}



int main()
{
    BinopPrecedence['<'] = 10;
    BinopPrecedence['+'] = 20;
    BinopPrecedence['-'] = 30;
    BinopPrecedence['*'] = 40;

    // Prime the first token.
    fprintf(stderr, "ready> ");
    getNextToken();
    
    // Run the main "interpreter loop" now.
    MainLoop();
    return 0;
}
