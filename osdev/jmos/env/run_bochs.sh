#!/bin/bash

sudo losetup /dev/loop0 floppy.img
sudo bochs -f bochssrc.txt
sudo losetup -d /dev/loop0
