#!/bin/bash
set -e
set -o pipefail
export reset="\e[0m"
export red="\e[1;31m"
export green="\e[32m"
module load boost/1.59
module load perl/5.22.1
module load bowtie/2.2.6


export APPS_SRC="${WORK}/apps"
export TOPHAT_SRC="${APPS_SRC}/tophat"                                             
export TOPHAT_VERSION="v2.1.1"
export TOPHAT_PATH="${TOPHAT_SRC}/${TOPHAT_VERSION}"
export TOPHAT_PKG_VER="tophat-2.1.1"
# Use Intel compilers to install tophat.
export CC=`which icc`
export CXX=`which icpc`
# Compilation (optimization) flags for Haswell/Ivy Bridge nodes.
export CFLAGS="-xAVX -axCORE-AVX2"
export CXXFLAGS="-xAVX -axCORE-AVX2"
export LDFLAGS="-xAVX -axCORE-AVX2"

 
# Create clone and get specific version from tags.
(
    mkdir -p "${APPS_SRC}" "${TOPHAT_PATH}" && cd "${APPS_SRC}" && \
    wget --no-check-certificate "https://ccb.jhu.edu/software/tophat/downloads/${TOPHAT_PKG_VER}.tar.gz" && \
        tar -xvzf "${TOPHAT_PKG_VER}.tar.gz" && \
        mv "${TOPHAT_PKG_VER}" "${TOPHAT_PATH}" 
)
# The bellow approach returns the following error:
# configure: error: cannot find sources (config.h.in) in . or ..
#(    
#    mkdir -p "${APPS_SRC}" && cd "${APPS_SRC}" && \
#    git clone https://github.com/infphilo/tophat "${TOPHAT_PATH}" && \
#    cd "${TOPHAT_PATH}" && \
#    git fetch origin --tags --prune && \
#    git checkout "${TOPHAT_VERSION}" && \
#    aclocal && autoconf && automake --add-missing
#)


# Configure tophat - take advantage of the Intel processors.
(
    cd "${TOPHAT_PATH}" && \
        ./configure --prefix="${TOPHAT_PATH}" \
        --with-boost="${TACC_BOOST_DIR}" \
        --enable-intel64 && \
        make && \
        make install && \
        echo -e "${green}Check ${TOPHAT_PATH}/bin...${reset}"
)
