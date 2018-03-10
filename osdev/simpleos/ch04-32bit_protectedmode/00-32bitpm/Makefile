all: boot_sect.asm gdt.asm print_string.asm print_string_pm.asm switch_to_pm.asm
	nasm $< -f bin -o boot_sect.bin

run: all
	qemu-system-i386 -fda boot_sect.bin

clean:
	rm -f boot_sect.bin
