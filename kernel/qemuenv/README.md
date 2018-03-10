# Development Environment for Linux 2.6.X

* http://archive.debian.org/debian/
 ```
 sudo debootstrap --variant=buildd --arch amd64 squeeze $DIR http://archive.debian.org/debian/
 ```

## Some variables to start off                            
```                                                    
IMG=qemu-image.img 
DIR=mount-point.dir
```

## Create image for qemu to boot
```
qemu-img create $IMG 1g
mkfs.ext2 $IMG 

mkdir $DIR
sudo mount -o loop $IMG $DIR
sudo debootstrap --variant=buildd --arch amd64 wheezy $DIR                                                                     
```

## Move src code and get in place to build
```                                                              
# Do whatever you need to get the src into the chroot
sudo cp -r ../../linux-2.6.39/ mount-point.dir/home/
```

## Build kernel
```
sudo chroot $DIR
cd /home

make x86_64_defconfig
make -j 8
```

## Get the kernel
```
cp mount-point.dir/home/linux-2.6.39/arch/x86/boot/bzImage . 
```

## Cleanup
```
sudo umount $DIR
```
