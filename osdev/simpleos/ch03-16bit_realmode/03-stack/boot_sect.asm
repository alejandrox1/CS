;
; Simple boot sector to demonstrate stack usage.
;
mov ah, 0x0e    ; int 10/ah = 0eh -> scrolling teletype BIOS routine.

mov bp, 0x8000   ; Set the base of the stack a little above where the BIOS loads 
mov sp, bp       ; the boot sector.

push 'o'        ; Push chars into stack. These will be pushed as 16-bit values,
push 'l'        ; so the most significant byte will be added by assembler as 
push 'l'        ; 0x00.
push 'e'
push 'h'

pop bx          ; We are poping 16-bit values, so we copy the 8-bit part to al.
mov al, bl
int 0x10

pop bx
mov al, bl
int 0x10

pop bx
mov al, bl
int 0x10

pop bx
mov al, bl
int 0x10

pop bx
mov al, bl
int 0x10

jmp $

; Padding and magic BIO number.
times 510-($-$$) db 0
dw 0xaa55
