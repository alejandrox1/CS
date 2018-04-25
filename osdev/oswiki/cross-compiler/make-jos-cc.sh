#!/bin/bash
#
# make-jos-cc.sh -  Build a GCC cross-compiler for target platform TARGET.
#
# 1. Set up environment.
#    - Make cross/ and src/ subdirs.
# 2. Download GNU Bintuils and GNU GCC.
# 3. Make cross-compiler.
#    - Make and install Binutils and GCC.
#
# Taken from https://wiki.osdev.org/GCC_Cross-Compiler
#
GRE="\e[32m"
RED="\e[31m"
NOC="\e[0m"

set -e

###############################################################################
##                        Environment and Variables                          ##
###############################################################################
# Version for building cross-compiler.
TARGET=i386-jos-elf
BINUTILS_VER="2.24"
GCC_VER="5.4.0"
GDB_VER="7.11.1"

BINUTILS="binutils-${BINUTILS_VER}"
GCC="gcc-${GCC_VER}"
GDB="gdb-${GDB_VER}"

# Build environment
BASE="$(pwd)/jos-cc-${GCC_VER}-${BINUTILS_VER}"
CROSS="${BASE}/cross"
BIN="${CROSS}/bin"
SRC="${BASE}/src"
BD_BINUTILS="${SRC}/build-binutils"
BD_GCC="${SRC}/build-gcc"
BD_GDB="${SRC}/build-gdb"

# Ensure compiler build is able to detect our new binutils once it has been     
# built.
export PATH="${BIN}:${PATH}"

###############################################################################
##                         Set up environment                                ##
###############################################################################
# Set up build directories.
if [ -d "$CROSS" ]; then rm -rf "${CROSS}"; fi
mkdir -p "$CROSS" "$BIN"
if [ -d "$SRC" ]; then rm -rf "${SRC}"; fi
mkdir -p "$SRC" "$BD_BINUTILS" "$BD_GCC" "$BD_GDB"


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


###############################################################################
##                        Download and Install SRC                           ##
###############################################################################
# Get source code.
get_pkg "${BINUTILS}" "ftp://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VER}.tar.gz";
get_pkg "${GCC}" "ftp://ftp.gnu.org/gnu/gcc/gcc-${GCC_VER}/gcc-${GCC_VER}.tar.gz";
get_pkg "${GDB}" 

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
) && \
(
    cd "${BD_GDB}";
    ./configure --target=${TARGET} --program-prefix="${CROSS}" --disable-werror && \
        make && \
        make install && \
        echo -e "${GRE}>>> Finished making ${GDB}.\n${NOC}";
)

# Install dependencies for grub-mkrescue.
sudo apt-get update -y && sudo apt-get install -y grub-pc-bin xorriso
