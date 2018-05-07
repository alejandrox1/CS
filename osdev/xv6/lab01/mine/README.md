# Booting Process
```
git clone http://pdos.csail.mit.edu/6.828/2011/jos.git lab
```

## Bootstrap
Edit `conf/env.mk` - for now, we can get away with the host's GCC tools though
we may have to specify the path for the QEMU executable,
```
QEMU=/usr/bin/qemu-system-i386
```

After this just make.
