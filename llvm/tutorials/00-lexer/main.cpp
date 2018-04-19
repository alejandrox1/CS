#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <string>

/*
 * Token.
 * Lexer return tokens [0-255] if it is an unkown character, otherwise one of
 * these for known things.
 */
enum Token {
    tok_eof = -1,

    // Commands.
    tok_def = -2,
    tok_extern = -3,

    // Primary.
    tok_identifier = -4,
    tok_number = -5,
};

static std::string IdentifierStr; // Filled in if tok_identifier.
static double NumVal;             // Filled in if tok_number.

/*
 * gettok (lexer) returns the next token from standard input.
 */
static int gettok() {
    static int LastChar = ' ';

    // Skip nay white spaces.
    while (isspace(LastChar))
        LastChar = getchar();

    // Identifier: [a-zA-Z][a-zA-Z0-9]*
    if (isalpha(LastChar)) {
        IdentifierStr = LastChar;
        while (isalnum((LastChar = getchar())))
            IdentifierStr += LastChar;

        if (IdentifierStr == "def")
            return tok_def;
        if (IdentifierStr == "extern")
            return tok_extern;
        return tok_identifier;
    }

    // Number: [0-9].+
    if (isdigit(LastChar) || LastChar == '.') {
        std::string NumStr;
        do {
            NumStr += LastChar;
            LastChar += getchar();
        } while (isdigit(LastChar) || LastChar == '.');

        NumVal = strtod(NumStr.c_str(), nullptr);
        return tok_number;
    }

    if (LastChar == '#') {
        // Comment until end of line.
        do
            LastChar = getchar();
        while (LastChar != EOF && LastChar != '\n' && LastChar != '\r');

        if (LastChar != EOF)
            return gettok();
    }

    // Check for end of file. Don't eat the EOF.
    if (LastChar == EOF)
        return tok_eof;

    // [Operatores] Otherwise, return character as its ASCII value.
    int ThisChar = LastChar;
    LastChar = getchar();
    return ThisChar;
}

static int CurTok;
static int getNextToken() { return CurTok = gettok(); }

static void MainLoop() {
    while (true) {
        fprintf(stderr, "ready> ");
        switch (CurTok) {
        case tok_eof:
            fprintf(stderr, "\tEOF\n");
            return;
        case ';': // ignore top-level semicolons.
            fprintf(stderr, "\tTop-level semicolon.\n");
            getNextToken();
            break;
        case tok_def:
            getNextToken(); // eat def
            fprintf(stderr, "Parsed a function definition.\n");
            fprintf(stderr, "\tFunc def: %s.\n", IdentifierStr.c_str());
            break;
        case tok_extern:
            fprintf(stderr, "\tExtern: %s.\n", IdentifierStr.c_str());
            getNextToken();
            break;
        case tok_identifier:
            fprintf(stderr, "\tIdetifier: %s.\n", IdentifierStr.c_str());
            getNextToken();
            break;
        case tok_number:
            fprintf(stderr, "\tNumber: %f.\n", NumVal);
            getNextToken();
            break;
        default:
            getNextToken();
            break;
        }
    }
}

int main() {
    fprintf(stderr, "ready> ");
    getNextToken();
    MainLoop();

    return 0;
}
