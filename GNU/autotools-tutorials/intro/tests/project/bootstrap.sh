#!/bin/bash

set -e

test -d autom4te.cache && rm -rf autom4te.cache

# Collect all macro invocations in configure.in needed to build a configure
# script. This will generate a file called 'aclocal.m4'.
aclocal

# Run autoconf to generate the configure script in the current local directory
autoconf

# GNU Automake requires these...
touch AUTHORS NEWS README ChangeLog

# Generate a Makefile.in file.
automake --add-missing
