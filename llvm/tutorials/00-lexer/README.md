# [Lexer](https://llvm.org/docs/tutorial/LangImpl01.html)


Using the lexer and trying it out with something as simple as `def foo()` will
retur the following output:
```
ready> def foo()
ready> Parsed a function definition.
    Func def: foo.
    ready>  Idetifier: foo.
    ready> ready>
```

This is just a little taste, just to see how th eexer works. The problem right
now is that we are walking along the source code one toke at a time so we have
no way of knowing whether the `foo` goes with the `def` or if it's just another
identifier.

## Lexical Analysis (Scanning)
Read the stream of characters making up the source program and group the
characters into meaningful chunks called lexemes.
For each lexeme, the lexical analyzer produces as output a token composed of
the token name and an attribute.

## Notes
Method `getNextToken` is a wrapper for `gettok`, it calls the latter and stores
the result in the `CurTok` (what type of toeken did we get?) container.

`gettok` in turn reads through a stream of characters testing various
conditions to determine whether the token corresponds to a token defined in the
enum `Token` and sets the corresponding valu for the `IdentifierStr` and
`NumVal` containers as needed - this hold the actual value of the token.

