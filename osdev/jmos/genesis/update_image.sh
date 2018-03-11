#!/bin/bash

set -e

if [ -f src/kernel ]; then
	mkdir -p mnt/ && cp src/kernel mnt/kernel; 
fi

 
sudo losetup /dev/loop0 floppy.img
sudo mount /dev/loop0 mnt/
sudo cp src/kernel /mnt/kernel
sudo umount /dev/loop0
sudo losetup -d /dev/loop0 
