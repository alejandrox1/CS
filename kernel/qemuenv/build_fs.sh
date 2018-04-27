#!/bin/bash

set -e

ARCH=amd64
DIST=xenial
IMG=vmroot_${DIST}_${ARCH}.img
DIR=mount_point

if [ -f ${IMG} ]; then rm ${IMG}; fi

qemu-img create $IMG 1g
mkfs.ext2 $IMG


mkdir -p $DIR
sudo mount -o loop $IMG $DIR
sudo debootstrap --variant=buildd --arch ${ARCH} ${DIST} ${DIR}
sudo chroot ${DIR} /bin/bash -c "passwd"
sudo umount ${DIR}
rmdir ${DIR}
