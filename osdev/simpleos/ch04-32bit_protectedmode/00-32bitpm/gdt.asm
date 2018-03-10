;
; GDT
;
gdt_start:

 gdt_null: ; Mandatory null descriptor.
 	dd 0x0 ; define double word (4 bytes)
	dd 0x0

 gdt_code: ; Code segment descriptor.
 	; base=0x0 , limit=0xfffff,
	; 1st flags: (present)1 (privileged)00 (descriptor type)1 -> 1001b
	; type flags: (code)1 (conforming)0 (readable)1 (accessed)0 -> 1010b
	; 2nd flags: (granularity)1 (32-bit default)1 (64-bit seg)0 (AVL)0 -> 1100b
	dw 0xffff    ; Limit (bits 0-15)
	dw 0x0       ; Base (bits 0-15)
	db 0x0       ; Base (bits 16-23)
	db 10011010b ; 1st flags, type flags
	db 11001111b ; 2nd flags, Limit (bits 16-19)
	db 0x0       ; Base (bits 24-31)

 gdt_data: ; Data segment descriptor.
 	; Similar to code segment except for type flags:
	; type flags: (code)0 (expand down)0 (writable)1 (accessed)0 -> 0010b
	dw 0xffff    ; Limit (bits 0-15)
	dw 0x0       ; Base (bits 0-15)
	db 0x0       ; Base (bits 16-23)
	db 10010010b ; 1st type flags, type flags
	db 11001111b ; 2nd flags, Limit (bits 16-19)
	db 0x0       ; Base (bits 24-31)
	
 gdt_end: ; The reason for including an end label for a GDT is to enable the 
          ; assembler to calculate the size of the GDT for a GDT descriptor, 
		  ; see below.

; GDT descriptor
gdt_descriptor:
	dw gdt_end - gdt_start - 1 ; Size of our GDT, always one less than the 
	                           ; the actual size (16-bit).
	dd gdt_start               ; Start address of our GDT (32-bit).

; Useful constants for the GDT segment descriptor offsets, which are what 
; segment registers must contain when in protected mode.
; For example, when we set DS = 0x10 in PM, the cpu knows that we mean to use 
; the segment described at offset 0x10 (16 bytes) in our GDT, which in our case
; is in the data segment (0x0 -> NULL, 0x08 -> CODE, 0x10 -> DATA).
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
