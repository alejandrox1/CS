#!/bin/bash
set -e
set -o pipefail

[ -f ubuntu.simg ] && rm -f ubuntu.simg

sudo singularity build ubuntu.simg Singularity && \
    ./ubuntu.simg
