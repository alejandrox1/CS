#!/bin/bash

set -e

qemu-system-x86_64 -kernel linux/arch/x86/boot/bzImage \
    -hda vmroot_xenial_amd64.img \
    -append "root=/dev/sda single"
