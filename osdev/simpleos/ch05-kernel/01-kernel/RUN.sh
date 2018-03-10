#!/bin/bash

gcc -ffreestanding -c kernel.c -o kernel.o &&\
	ld -o kernel.bin -Ttext 0x1000 --oformat binary kernel.o &&\
	nasm -o boot_sect.bin -f bin boot_sect.asm &&\
	cat boot_sect.bin kernel.bin > os-image &&\
	qemu-system-i386 -fda os-image
