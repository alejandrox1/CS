#!/bin/bash
set -e
set -o pipefail

sudo singularity build ubuntu.simg Singularity && \
    ./ubuntu.simg
