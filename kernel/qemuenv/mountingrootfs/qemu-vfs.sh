#!/bin/bash
#
# Mount a qemu image file on a diectory and use debootstrap in the mount point.
#

IMG=qemu-image.img
DIR=mount-point.dir

# CReate image.
qemu-img create $IMG 1g

# Create an ext2 filesystem. 
mkfs.ext2 $IMG 

mkdir $DIR
sudo mount -o loop $IMG $DIR 
sudo debootstrap --arch amd64 jessie $DIR
sudo umount $DIR
rmdir $DIR
