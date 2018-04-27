#!/bin/bash

set -e

# If interested in a particular version
# curl -C - -O https://mirrors.edge.kernel.org/pub/linux/kernel/v4.x/linux-4.16.5.tar.gz
# or wget -c https://mirrors.edge.kernel.org/pub/linux/kernel/v4.x/linux-4.16.5.tar.gz

git clone --depth=1 git://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git && \
    cd linux && \
    make x86_64_defconfig && \
    make kvmconfig && \
    make -j 8

