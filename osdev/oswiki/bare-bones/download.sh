#!/bin/bash

GRE="\e[32m"
RED="\e[31m"
NOC="\e[0m"

set -e

# Version for building cross-compiler.
BINUTILS_VER="2.23"
GCC_VER="5.4.0"

BD_BINUTILS=$(pwd)/build-binutils
BD_GCC=$(pwd)/build-gcc


# Set up build directories.
if [ -d $BD_BINUTILS ]; then rm -rf ${BD_BINUTILS}; fi
mkdir $BD_BINUTILS
if [ -d $BD_GCC ]; then rm -rf ${BD_GCC}; fi
mkdir $BD_GCC

# Get source code.
if command -v curl> /dev/null 2>&1; then
    echo -e "${GRE}Downloading Binutils-${BINUTILS_VER}...\n${NOC}"
    (
        cd ${BD_BINUTILS};
        curl -C - -O ftp://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VER}.tar.gz
    )
    echo -e "${GRE}Downloading GCC-${GCC_VER}...\n${NOC}"
    (
        cd ${BD_GCC};
        curl -C - -O ftp://ftp.gnu.org/gnu/gcc/gcc-${GCC_VER}/gcc-${GCC_VER}.tar.gz
    )

elif command -v wget > /dev/null 2>&1; then
    echo -e "${GRE}Downloading Binutils-${BINUTILS_VER}...\n${NOC}"
    (
        cd ${BD_BINUTILS};
        wget -c ftp://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VER}.tar.gz
    )
    echo -e "${GRE}Downloading GCC-${GCC_VER}...\n${NOC}"
    (
        cd ${BD_GCC};
        wget -c ftp://ftp.gnu.org/gnu/gcc/gcc-${GCC_VER}/gcc-${GCC_VER}.tar.gz
    )

else
    echo -e "${RED}Neither Curl or wget are currently installed...\n${NOC}"
    exit 1
fi
