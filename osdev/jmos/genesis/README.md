# Boot with a Minimal Kernel

## Running
```
qemu-system-i386 -fda floppy.img
```

Reality check
```
qemu-system-i386 -fda floppy.img -boot a -s -S
```
then on another terminal,
```
$ gdb

target remote localhost:1234
c
info registers
```

This should give output something like this,
```
eax            0xdeadbaba	-559039814
ecx            0x1edd0	126416
edx            0x1	1
ebx            0x2d000	184320
esp            0x67ec8	0x67ec8
ebp            0x67ee0	0x67ee0
esi            0x53c8e	343182
edi            0x53c8f	343183
eip            0x100027	0x100027
eflags         0x2	[ ]
cs             0x8	8
ss             0x10	16
ds             0x10	16
es             0x10	16
fs             0x10	16
gs             0x10	16
```
