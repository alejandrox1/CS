#!/bin/bash
set -e
set -o pipefail

SRC_PATH="apps"
TOPHAT_VERSION="v2.1.1"

module load boost/1.59
module load perl/5.22.1
module load bowtie/2.2.6


# Intall app on $WORK.
cdw

# Create clone and get specific version from tags.
(    
    mkdir -p "${SRC_PATH}" && \
    cd "${SRC_PATH}" && \
    git clone https://github.com/infphilo/tophat && \
    cd tophat && \
    git fetch origin --tags --prune && \
    git checkout "${TOPHAT_VERSION}"
)
