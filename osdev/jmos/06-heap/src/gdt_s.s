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
