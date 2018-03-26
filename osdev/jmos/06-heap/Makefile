CSOURCES=$(shell find src/ -name *.c)
COBJECTS=$(patsubst %.c, %.o, $(CSOURCES))
SSOURCES=$(shell find src/ -name *.s)
SOBJECTS=$(patsubst %.s, %.o, $(SSOURCES))

cc=gcc
LD=ld
CFLAGS=-Wall -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector
LDFLAGS=-m elf_i386 -T link.ld
ASFLAGS=-f elf


all: $(COBJECTS) $(SOBJECTS) link types

link:
	@echo Linking
	@$(LD) $(LDFLAGS) -o kernel $(SOBJECTS) $(COBJECTS)

.s.o:
	@echo Asembling $<
	@nasm $(ASFLAGS) $<

.c.o:
	@echo Compiling $<
	@$(CC) $(CFLAGS) -o $@ -c $<

types: types.vim

types.vim: src/*.[ch]
	ctags --c-kinds=gstu -o- src/*.[ch] |\
		awk 'BEGIN{printf("syntax keyword Type\t")}\
		{printf("%s ", $$1)}END{print ""}' > $@
	mv types.vim src/types.vim


clean:
	rm -f $(SOBJECTS) $(COBJECTS) kernel
