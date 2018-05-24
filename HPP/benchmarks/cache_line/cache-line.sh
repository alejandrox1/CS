#!/bin/bash

set -e

CLRES="results.txt"

if [ -f "${CLRES}" ]; then
    echo Deleting "${CLRES}" && rm "${CLRES}"
fi

make clean && make line_size && \
    for ts in 2 4 8 16 32 64 128 256; do
        ./line_size "${ts}" | tee -a "${CLRES}" 
    done && \
    gnuplot -p plot.gp

make clean
