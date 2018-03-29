;
; boot.s 
;
; Kernel start location.
; Also defines multiboot header.
; From James Molloy's kernel development tutorial, section genesis.
;
MBOOT_PAGE_ALIGN   equ 1<<0       ; Load kernel and modules on a page boundary.
MBOOT_MEM_INFO     equ 1<<1       ; Provide your kernel with memory info.
MBOOT_HEADER_MAGIC equ 0x1BADB002 ; Multiboot magic number. Identifies kernel 
                                  ; multiboot compatible.
; We do not use MBOOT_ABOUT_KLUDGE. This means GRUB does not pass us a symbol 
; table. Because we willbe using elf format, this is not necessary and adding 
; it would stop GRUB from giving us a symbol table when we boot up.
MBOOT_HEADER_FLAGS equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM     equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

[BITS 32]

[GLOBAL mboot] ; Make mboot accessible from C.
[EXTERN code]  ; Start of the .text section.
[EXTERN bss]   ; Start of the .bss section.
[EXTERN end]   ; End of the last loadble section.

mboot:
	dd MBOOT_HEADER_MAGIC ; GRUB will search for this value on each 4-byte 
	                      ; boundary in the kernel file. Identifies kernel as 
                          ; multiboot compatible
	dd MBOOT_HEADER_FLAGS ; How GRUB should load file/settings.
	dd MBOOT_CHECKSUM     ; Ensure the above values are correct.

	dd mboot              ; Location of this descriptor
	dd code               ; Start the kernel .text section.
	dd bss                ; End of kernel .data section.
	dd end                ; End of kernel.
	dd start              ; Kernel entrypoint (initial EIP).

[GLOBAL start]            ; Kernel entrypoint.
[EXTERN kmain]             ; Entrypoint for C code.

start:
	push ebx              ; Load multiboot header lcoation.

	; Execute the kernel
	cli                   ; Disable interrupts.
	call kmain             ; Call our kmain() func.
	jmp $
