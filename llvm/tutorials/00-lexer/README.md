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
