CFORMAT=clang-format-6.0

CC?=gcc
CFLAGS?=-Wall -Wextra

.PHONY: helloflops2 format clean

helloflops2: helloflops2.c
	@echo -e "$${green} + Building $@ ...$${reset}"
	$(CC) $(CFLAGS) -o $@ $^

format:
	@echo + Formatting src...
	find . -name "*.c" -o -name "*.h" | xargs $(CFORMAT) -i

clean:
	@echo -e "$${green} + Removing executables...$${reset}"
	rm -f helloflops2
