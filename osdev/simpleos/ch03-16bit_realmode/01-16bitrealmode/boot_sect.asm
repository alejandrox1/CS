;
; A simple boot sector that rpints a message to screen device using a BIOS 
; rutine.
; 

mov ah, 0x0e    ; int 10/ah = 0eh -> scrolling teletype BIOS rutine.
mov al, 'H'
int 0x10
mov al, 'e'
int 0x10
mov al, 'l'
int 0x10
mov al, 'l'
int 0x10
mov al, 'o'
int 0x10

jmp $           ; Jump to the current address (i.e. forever).


;
; Padding and magic BIOS number.
;
times 510-($-$$) db 0
dw 0xaa55

