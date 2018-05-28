#!/bin/bash
set -e
set -o pipefail
export reset="\e[0m"
export red="\e[1;31m"
export green="\e[32m"
module load boost/1.59
module load perl/5.22.1
module load bowtie/2.2.6


# Intall app on $WORK.                                                          
cdw

export TOPHAT_SRC_PATH="apps"                                                                 
export TOPHAT_VERSION="v2.1.1"
export TOPHAT_PATH="${TOPHAT_SRC_PATH}/tophat/${TOPHAT_VERSION}"
# Use Intel compilers to install tophat.
export CC=`which icc`
export CXX=`which icpc`
# Compilation (optimization) flags for Haswell/Ivy Bridge nodes.
export CFLAGS="-xAVX -axCORE-AVX2"
export CXXFLAGS="-xAVX -axCORE-AVX2"
export LDFLAGS="-xAVX -axCORE-AVX2"


# Create clone and get specific version from tags.
(    
    mkdir -p "${TOPHAT_SRC_PATH}" && \
    cd "${TOPHAT_SRC_PATH}" && \
    git clone https://github.com/infphilo/tophat && \
    cd tophat && \
    git fetch origin --tags --prune && \
    git checkout "${TOPHAT_VERSION}"
)

# Configure tophat - take advantage of the Intel processors.
(
    ./configure --prefix="${TOPHAT_PATH}" \
        --with-boost="${TACC_BOOST_DIR}" \
        --enable-intel64 && \
        make && \
        make install && \
        echo -e "${green}Check ${TOPHAT_PATH}/bin...${reset}"
)
