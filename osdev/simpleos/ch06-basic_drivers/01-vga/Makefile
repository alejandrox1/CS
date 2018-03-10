C_SRC=$(wildcard kernel/*.c drivers/*.c)
HEADERS=$(wildcard kernel/*.h drivers/.*h)
OBJ=${C_SRC:.c=.o}


all: os-image

run: all
	qemu-system-i386 -fda os-image

os-image: boot/boot_sect.bin kernel/kernel.bin
	cat $^ > $@

# Build the kernel binary
kernel/kernel.bin: kernel/kernel_entry.o ${OBJ}
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

# Build the kernel object file
%.o: %.c ${HEADERS}
	gcc -m32 -ffreestanding -c $< -o $@

# Assemble kernel entry object file
%.o: %.asm 
	nasm $< -f elf -o $@

# Build boot sector
%.bin: %.asm
	nasm $< -f bin -I './boot/' -o $@


clean:
	rm -f *.o *.bin os-image
	rm -f kernel/*.o drivers/*.o boot/*.bin kernel/*.bin
