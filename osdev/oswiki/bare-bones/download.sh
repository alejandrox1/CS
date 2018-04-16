#!/bin/bash

GRE="\e[32m"
RED="\e[31m"
NOC="\e[0m"

set -e

# Version for building cross-compiler.
BINUTILS_VER="2.23"
GCC_VER="5.4.0"

TARGET=i686-elf
BINUTILS="binutils-${BINUTILS_VER}"
GCC="gcc-${GCC_VER}"

CROSS="$(pwd)/cross"
BIN="$(pwd)/cross/bin"
SRC="$(pwd)/src"
BD_BINUTILS="${SRC}/build-binutils"
BD_GCC="${SRC}/build-gcc"

# Ensure compiler build is able to detect our new binutils once it has been     
# built.
export PATH="${BIN}:${PATH}"


# Set up build directories.
if [ -d "$CROSS" ]; then rm -rf "${CROSS}"; fi
mkdir -p "$CROSS" "$BIN"
if [ -d "$SRC" ]; then rm -rf "${SRC}"; fi
mkdir -p "$SRC" "$BD_BINUTILS" "$BD_GCC"


get_pkg() {
    LOG_FILE="${1}.log"
    echo -e "${GRE}>>> Downloading ${1}...${NOC}";
    (
        cd "${SRC}";
        if command -v curl> /dev/null 2>&1; then
            curl -C - -O "${2}" > ${LOG_FILE} 2>&1 && \
                tar -xzf "${1}.tar.gz" >> ${LOG_FILE} 2>&1;
        elif command -v wget > /dev/null 2>&1; then
            wget -c "${2}" > ${LOG_FILE} 2>&1 && \
                tar -xzf "${1}.tar.gz" >> ${LOG_FILE} 2>&1;
        else
            echo -e "${RED}>>> Neither curl nor wget are currently installed...\n${NOC}";
            exit 1
        fi
    )
    echo -e "${GRE}>>> Finished unpacking ${1}.\n${NOC}";
}




# Get source code.
get_pkg "${BINUTILS}" "ftp://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VER}.tar.gz";
get_pkg "${GCC}" "ftp://ftp.gnu.org/gnu/gcc/gcc-${GCC_VER}/gcc-${GCC_VER}.tar.gz";

# Build cross-compiler.
(
    cd "${BD_BINUTILS}";
    ../${BINUTILS}/configure --target="${TARGET}" --prefix="${CROSS}" --with-sysroot --disable-nls --disable-werror &&\
        make && \
        make install && \
        echo -e "${GRE}>>> Finished making ${BINUTILS}.\n${NOC}";
) && \
(
    which -- ${TARGET}-as || echo ${TARGET}-as is not in the PATH && \
        cd "${BD_GCC}" && \
        ../${GCC}/configure --target=${TARGET} --prefix="${CROSS}" --disable-nls --enable-languages=c,c++ --without-headers && \
        make all-gcc && \
        make all-target-libgcc && \
        make install-gcc && \
        make install-target-libgcc && \
        echo -e "${GRE}>>> Finished making ${GCC}.\n${NOC}";
)
