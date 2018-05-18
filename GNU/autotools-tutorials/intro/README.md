# [Building a GNU Autotools Project](http://inti.sourceforge.net/tutorial/libinti/autotoolsproject.html)

`Autoconf` and `Automake` provide an effective build system to maintain
software.
`Automake` examines source files, determines how they depend on each other, and
generates a Makefile so the files can be compiled in correct order.
`Autoconf` permits automatic configuration of software insallation, handling a
large number of system details to improve portability.

## Getting Started
### Essential File
The smallest project requires two files:
1. `Makefile.am` - an input file to automake that specifies a project's build
   requirements: what needs to be built, and where it goes when installed.

2. `configure.in` - an input file to autoconf that provides the macro
   invocations and shell code fragments that autoconf uses to build a configure
   script.

### Project Structure
* Top-level directory is used for configuration files, chnage logs, README,
  etc.

* Any unique library should have its own subdir containing all headers and
  sources, a Makefile.am, any any other library-specific files.

* The headers and sources for the main application should be in the subdir
  `src`.

* Other subdirs can include `config` for intermediate files, `doc` for the
  project's documentation, and `test` for the projects self-test suite.


## Building and Installing the Project
Run `bootsratp.sh` and then,
```bash
# Find a place for the bin dir
./configure --prefix=<some dir>
make
make install
```

To install without debugging symbols install with `make install-strip`.
