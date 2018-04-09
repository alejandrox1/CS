;
; gdt.s
;
; Setup the GDT.
; Taken from JamesM' kernel development tutorial.
;
[GLOBAL gdt_flush]      ; Allow kernel to call this routine.

gdt_flush:
    mov eax, [esp+4]    ; Get the pointer to the GDT (uint32_t) passed as 
                        ; parameter.
    lgdt [eax]

	; Setup segments.
	; GDT structure (8 bytes per entry): 0x0 -> NULL, 0x08 -> CODE, 0x10 -> DATA.
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:.flush
.flush:
    ret

[GLOBAL tss_flush]
tss_flush:
    mov ax, 0x2B    ; Load index of our TSS structure - index is 0x2B, as it is
                    ; the 5th selector and each is 8 bytes long, but we set the
                    ; bottom 2 bits, making 0x2B.
    ltr ax          ; Load index into the task state register.
    ret
