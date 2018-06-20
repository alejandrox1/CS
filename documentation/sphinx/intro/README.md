# Sphin Tutorial

Run `sphinx-quickstart`. Make sure to enable `dautodoc` extensions!

You should end up with something like this (`trianglelib` is the sample library
we will be developing/documenting).
```
$ tree
.
├── doc
│   ├── _build
│   ├── conf.py
│   ├── index.rst
│   ├── Makefile
│   ├── _static
│   └── _templates
├── setup.py
└── trianglelib
    ├── __init__.py
    ├── shape.py
    └── utils.py
```

To test drive it
```
cd doc && make html && python -m http.server
```
On your browser go to `http://localhost:8000`
