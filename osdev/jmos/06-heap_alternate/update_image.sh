#!/bin/bash

set -x
set -e

make  

if [ -f kernel ]; then
	mkdir -p mnt/ #&& cp kernel mnt/kernel; 
fi

 
sudo losetup /dev/loop0 floppy.img
sudo mount /dev/loop0 mnt/
sudo cp kernel mnt/kernel
sudo umount /dev/loop0
sudo losetup -d /dev/loop0

rm -r mnt
make clean 
