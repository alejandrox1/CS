# Boot with a Minimal Kernel

## Running on Qemu
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

## Running on Bochs

Some useful [commands](http://bochs.sourceforge.net/doc/docbook/user/internal-debugger.html)

For more configuration options: [here](http://bochs.sourceforge.net/doc/docbook/user/bochsrc.html)

```

``t t=0
(0) [0x0000fffffff0] f000:fff0 (unk. ctxt): jmpf 0xf000:e05b          ; ea5be000f0
<bochs:1> c
^CNext at t=2040658227
(0) [0x000000100027] 0008:0000000000100027 (unk. ctxt): jmp .-2 (0x00100027)      ; ebfe
<bochs:2> r
rax: 00000000_deadbaba rcx: 00000000_0001edd0
rdx: 00000000_00000001 rbx: 00000000_0002d000
rsp: 00000000_00067ec8 rbp: 00000000_00067ee0
rsi: 00000000_00053c8e rdi: 00000000_00053c8f
r8 : 00000000_00000000 r9 : 00000000_00000000
r10: 00000000_00000000 r11: 00000000_00000000
r12: 00000000_00000000 r13: 00000000_00000000
r14: 00000000_00000000 r15: 00000000_00000000
rip: 00000000_00100027
eflags 0x00000002: id vip vif ac vm rf nt IOPL=0 of df if tf sf zf af pf cf`
