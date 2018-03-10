#!/bin/bash

qemu-system-x86_64 -kernel ../bzImage -hda qemu-image.img -append "root=/dev/sda single"
