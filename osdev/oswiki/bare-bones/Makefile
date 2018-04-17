AS=cc/cross/bin/i686-elf-as
CC=cc/cross/bin/i686-elf-gcc
CFLAGS=-std=gnu11 -ffreestanding -O2 -Wall -Wextra
INFLAGS=-Iinclude -Idrivers
LDFLAGS=-ffreestanding -O2 -nostdlib

TARGET=myos.bin
TARGET_IMG=$(TARGET:.bin=.iso)
SSRC=$(shell find boot/ -name "*.s")
SOBJ=$(SSRC:.s=.o)
CSRC=$(shell find boot/ drivers/ include/ kernel/ lib/ -name "*.c")
COBJ=$(CSRC:.c=.o)

all: $(TARGET)
	grub-file --is-x86-multiboot $<

$(TARGET): $(SOBJ) $(COBJ)
	$(CC) -T linker.ld -o $@ $(LDFLAGS) $^ -lgcc

%.o: %.c
	$(CC) $(CFLAGS) $(INFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $< -o $@

image: all
	mkdir -p isodir/boot/grub && \
		cp ${TARGET} isodir/boot/${TARGET} && \
		cp grub.cfg isodir/boot/grub/grub.cfg && \
		grub-mkrescue -o ${TARGET_IMG} isodir

run: image
	qemu-system-i386 -cdrom $(TARGET_IMG)

clean:
	rm -f $(SOBJ)
	rm -f $(COBJ)
	rm -f $(TARGET)
	rm -f $(TARGET_IMG)
	rm -fr isodir
