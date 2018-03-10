;
; A simple boot sector program to demonstrate addressing.
;
mov ah, 0x0e    ; int 10/ah = 0eh -> scrolling teletype BIOS routine.

; First attempt
mov al, the_secret    ; load inmediate offset into al.
int 0x10              ; Does this print an X? No.

; Second attemp
mov al, [the_secret]  ; Indirect adressing - get the contents at the offset.
int 0x10              ; Does this print an X? No.

; Third attempt
mov bx, the_secret
add bx, 0x7c00        ; Same as before, just add offset from where this was loaded by BIOS.
mov al, [bx]
int 0x10              ; Does this print an X?

; Fourth attempt
mov al, [0x7c1d]      ; X was 29 bytes from the offset (see od dump). Look for ascii 0x58
int 0x10              ; Does this print an X?

jmp $                 ; Jump forever

the_secret:
 db "X"

;
; Padding and magic BIOS number
;
times 510-($-$$) db 0
dw 0xaa55
