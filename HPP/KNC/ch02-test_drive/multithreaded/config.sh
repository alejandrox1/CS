#!/bin/bash
set -e

export MAKE=${MAKE:-make}

if command -v icc > /dev/null 2>&1; then    # Intel compilation configuration.
    export CC=icc
    if [ "${NO_OPTIMIZATION}" == "true" ]; then
        export CFLAGS='-no-vec -no-simd -qopenmp'

    elif [ "${KNL}" == "true" ] || [ "${SKYLAKE}" == "true" ]; then
        export CFLAGS='-xMIC-AVX512 -qopt-report=3 -O3 -qopenmp'

    elif [ "${HASWELL}" == "true" ]; then
        export CFLAGS='-xAVX -qopt-report=3 -O3 -qopenmp'
    fi
elif command -v gcc > /dev/null 2>&1; then  # GCC compilation configuration.
    export CC=gcc
    if [ "${NO_OPTIMIZATION}" == "true" ]; then
        export CFLAGS='-Wall -Wextra -std=gnu11 -fopenmp'

    else
        export CFLAGS='-Wall -Wextra -fopenmp -Ofast -mavx -mfma -std=gnu11'
    fi
else
    echo -e "${red}Neither the Intel compilers nor GCC are available...${reset}"
    exit 1
fi
