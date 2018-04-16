#!/bin/bash

GRE="\e[32m"
RED="\e[31m"
NOC="\e[0m"

set -e

# Version for building cross-compiler.
BINUTILS_VER="2.23"
GCC_VER="5.4.0"

TARGET=i686-elf
CROSS="$(pwd)/cross"
BIN="$(pwd)/cross/bin"
SRC="$(pwd)/src"
BD_BINUTILS="${SRC}/build-binutils"
BD_GCC="${SRC}/build-gcc"
# Ensure compiler build is able to detect our new binutils once it has been     
# built.
export PATH="${BIN}:${PATH}"


get_binutils() {
    LOG_FILE="binutils.log"
    echo -e "${GRE}>>> Downloading Binutils-${BINUTILS_VER}...\n${NOC}";
    (
        cd "${SRC}";
        if command -v curl> /dev/null 2>&1; then
            curl -C - -O "ftp://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VER}.tar.gz" > ${LOG_FILE} 2>&1 && \
                tar -xzf "binutils-${BINUTILS_VER}.tar.gz" >> binutils.log 2>&1;
        elif command -v wget > /dev/null 2>&1; then
            wget -c "ftp://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VER}.tar.gz" > ${LOG_FILE} 2>&1 && \
                tar -xzf "binutils-${BINUTILS_VER}.tar.gz" >> binutils.log 2>&1;
        else
            echo -e "${RED}>>> Neither curl nor wget are currently installed...\n${NOC}";
            exit 1
        fi
    )
    echo -e "${GRE}>>> Finished unpacking Binutils-${BINUTILS_VER}${NOC}";
}

get_gcc() {
    LOG_FILE="gcc.log"
    echo -e "${GRE}>>> Downloading GCC-${GCC_VER}...\n${NOC}";
    (
        cd ${SRC};
        if command -v curl> /dev/null 2>&1; then
            curl -C - -O "ftp://ftp.gnu.org/gnu/gcc/gcc-${GCC_VER}/gcc-${GCC_VER}.tar.gz" > ${LOG_FILE} 2>&1 && \
                tar -xzf "gcc-${GCC_VER}.tar.gz" >> gcc.log 2>&1;
        elif command -v wget > /dev/null 2>&1; then
            wget -c "ftp://ftp.gnu.org/gnu/gcc/gcc-${GCC_VER}/gcc-${GCC_VER}.tar.gz" > ${LOG_FILE} 2>&1 && \
                tar -xzf "gcc-${GCC_VER}.tar.gz" >> gcc.log 2>&1;
        else
            echo -e "${RED}>>> Neither curl nor wget are currently installed...\n${NOC}";
            exit 1
        fi
    )
    echo -e "${GRE}>>> Finished unpacking GCC-${GCC_VER}...\n${NOC}";
}

# Set up build directories.
if [ -d "$CROSS" ]; then rm -rf "${CROSS}"; fi
mkdir -p "$CROSS" "$BIN"
if [ -d "$SRC" ]; then rm -rf "${SRC}"; fi
mkdir -p "$SRC" "$BD_BINUTILS" "$BD_GCC"


# Get source code.
get_binutils &
get_gcc &
wait

# Build cross-compiler.
# disable-nls : dible-nls : disable native language support                                 
# with-sysroot : enables sysroot support                                        
#../binutils-2.24/configure --target=$TARGET --prefix="$CROSS" --with-sysroot --disable-nls --disable-werror
#make                                                                            
#make install 
