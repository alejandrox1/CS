CC?=gcc
CFLAGS?=-Wall -Wextra

.PHONY: helloflops1 clean

helloflops1: helloflops1.c
	@echo + Building $@ ...
	$(CC) $(CFLAGS) -o $@ $^

clean:
	@echo + Removing executables...
	rm -f helloflops1
