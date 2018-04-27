# Linux Kernel Development Environmnet Based on QEMU 

This is assuming a Debian distro on an x86 architecture.

## Up and Running
To get started we simply need to build the kernel: `build_kernel.sh` will do
this for us.

In order to use QEMU to mess around with the kernel we willneed to create a
filesystem for the kernel to boot into and that is what `build_fs.sh` is for.
`build_fs.sh` will build a development environment from Ubuntu Xenial.

Finally, once you have a filesystem and a kernel, run `run-qemu.sh`.

Happy hacking :-)


## Development Environment for Linux 2.6.X
Going back in time is made posible with a couple hacks...

To get an older distro you will have to find a mirror containing all the
necessary source code, here is one for you:

* http://archive.debian.org/debian/
 ```
 sudo debootstrap --variant=buildd --arch amd64 squeeze $DIR http://archive.debian.org/debian/
 ```

### Move src code and get in place to build
If you have the environment needed to compile an old kernel version then you
can simply move your src into the chroot env:
```                                                              
# Get the src into the chroot
sudo cp -r ../../linux-2.6.39/ mount-point.dir/home/
```

### Build kernel
Inside the chroot build as normal,
```
sudo chroot $DIR
cd /home

make x86_64_defconfig
make -j 8
```
