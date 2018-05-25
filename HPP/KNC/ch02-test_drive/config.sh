#!/bin/bash
set -e

export MAKE=${MAKE:-make}

if command -v icc > /dev/null 2>&1; then    # Intel compilation configuration.
    export CC=icc
    if [ "${NO_OPTIMIZATION}" == "false" ]; then
        export CFLAGS='-mmic -vec-report=3 -O3'
    else
        export CFLAGS='-no-vec -no-simd'
    fi
elif command -v gcc > /dev/null 2>&1; then  # GCC compilation configuration.
    export CC=gcc
    if [ "${NO_OPTIMIZATION}" == "false" ]; then
        export CFLAGS='-Wall -Wextra -Ofast -mavx -mfma -std=gnu11'
    else
        export CFLAGS='-Wall -Wextra -std=gnu11'
    fi
else
    echo Neither the Intel compilers nor GCC are available...
    exit 1
fi
