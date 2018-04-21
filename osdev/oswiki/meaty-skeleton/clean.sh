#!/bin/bash
set -e
. ./config.sh

for PROJECT in $PROJECTS; do
    (cd $PROJECT && $MAKE clean);
done

rm -r sysroot
rm -r isodir
rm -r myos.iso
