;
; A simple boot sector program that loops forever.
;

loop:                       ; Define a label loop.
	
    jmp loop                ; Jump to address of current instruction.


times 510-($-$$) db 0       ; When compiled, our program must fit into 512 bytes,
                            ; with the last two bytes being the magic number.
                            ; Here, we tell our asembly compiler to pad out our
                            ; program with enough zero bytes (db 0) to bring us
                            ; to the 510th byte.

dw 0xaa55                   ; Tell BIOS this is a boot sector.
