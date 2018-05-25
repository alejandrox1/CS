#!/bin/bash
set -e

export MAKE=${MAKE:-make}

if command -v icc > /dev/null 2>&1; then    # Intel compilation configuration.
    export CC=icc
    if [ "${NO_OPTIMIZATION}" == "true" ]; then
        export CFLAGS='-no-vec -no-simd'
    else
        export CFLAGS='-qopt-report=3 -O3'
    fi
elif command -v gcc > /dev/null 2>&1; then  # GCC compilation configuration.
    export CC=gcc
    if [ "${NO_OPTIMIZATION}" == "true" ]; then
        export CFLAGS='-Wall -Wextra -std=gnu11'
    else
        export CFLAGS='-Wall -Wextra -Ofast -mavx -mfma -std=gnu11'
    fi
else
    echo -e "${red}Neither the Intel compilers nor GCC are available...${reset}"
    exit 1
fi
