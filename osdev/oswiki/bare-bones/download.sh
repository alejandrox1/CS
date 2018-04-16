#!/bin/bash

GRE="\e[32m"
RED="\e[31m"
NOC="\e[0m"

set -e

# Version for building cross-compiler.
BINUTILS_VER="2.23"
GCC_VER="5.4.0"

# Work direcories.
if [ -d build-binutils ]; then rm -rf build-binutils; fi
mkdir build-binutils
if [ -d build-gcc ]; then rm -rf build-gcc; fi
mkdir build-gcc

# Download binutils and gcc.
if command -v curl> /dev/null 2>&1; then
    echo -e "${GRE}Downloading Binutils-${BINUTILS_VER}...\n${NOC}"
    (
        cd build-binutils; 
        curl -C - -O ftp://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VER}.tar.gz
    )
    echo -e "${GRE}Downloading GCC-${GCC_VER}...\n${NOC}"
    (
        cd build-gcc;
        curl -C - -O ftp://ftp.gnu.org/gnu/gcc/gcc-${GCC_VER}/gcc-${GCC_VER}.tar.gz
    )

elif command -v wget > /dev/null 2>&1; then
    echo -e "${GRE}Downloading Binutils-${BINUTILS_VER}...\n${NOC}"
    wget -c ftp://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VER}.tar.gz
    echo -e "${GRE}Downloading GCC-${GCC_VER}...\n${NOC}"
    wget -c ftp://ftp.gnu.org/gnu/gcc/gcc-${GCC_VER}/gcc-${GCC_VER}.tar.gz

else
    echo -e "${RED}Neither Curl or wget are currently installed...\n${NOC}"
    exit 1
fi
