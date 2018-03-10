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
#sudo mount -o loop $IMG $DIR 
sudo debootstrap --variant=buildd --arch amd64 wheezy $DIR 
sudo cp -r ../../linux-2.6.39/ mount-point.dir/
#sudo umount $DIR
#rmdir $DIR
