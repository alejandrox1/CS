all: boot_sect.asm
	nasm $< -f bin -o boot_sect.bin

run: all
	qemu-system-i386 -fda boot_sect.bin

clean:
	rm -f boot_sect.bin
