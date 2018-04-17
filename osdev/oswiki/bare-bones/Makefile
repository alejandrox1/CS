AS=cc/cross/bin/i686-elf-as
CC=cc/cross/bin/i686-elf-gcc
CFLAGS=-std=gnu11 -ffreestanding -O2 -Wall -Wextra
INFLAGS=-Iinclude -Idrivers

SSRC=$(shell find boot/ -name "*.s")
SOBJ=$(SSRC:.s=.o)
CSRC=$(shell find boot/ drivers/ include/ kernel/ lib/ -name "*.c")
COBJ=$(CSRC:.c=.o)

kernel: $(COBJ)

%.o: %.c
	$(CC) $(CFLAGS) $(INFLAGS) -c $< -o $@

all: $(SOBJ)

%.o: %.s
	$(AS) $< -o $@

clean:
	rm -f $(SOBJ)
	rm -f $(COBJ)
