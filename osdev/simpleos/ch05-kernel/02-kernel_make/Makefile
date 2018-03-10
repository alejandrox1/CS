IMG=os-image

all: boot_sect.bin kernel.bin
	cat $^ > ${IMG}

run: all
	qemu-system-i386 -fda ${IMG}

# Build boot sect
boot_sect.bin: boot_sect.asm
	nasm $< -f bin -o $@

# Build the kernel binary
kernel.bin: kernel_entry.o kernel.o
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

# Build kernel entry object file
kernel_entry.o: kernel_entry.asm
	nasm $< -f elf -o $@

# Build the kernel object file
kernel.o: kernel.c
	gcc -m32 -ffreestanding -c $< -o $@

clean:
	rm -f *.o *.bin os-image
