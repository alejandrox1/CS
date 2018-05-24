#!/bin/bash
set -e

export MAKE=${MAKE:-make}
export CC=gcc
export CFLAGS='-Wall -Wextra -Ofast -mavx -mfma -std=gnu11'
