AS=cc/cross/bin/i686-elf-as
SSRC=$(shell find boot/ -name "*.s")
SOBJ=$(SSRC:.s=.o)

all: $(SOBJ)

%.o: %.s
	$(AS) $< -o $@

clean:
	rm -f $(SOBJ)
