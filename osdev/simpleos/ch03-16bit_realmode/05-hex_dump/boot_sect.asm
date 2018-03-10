;
; A boot sector that prints "Hello World."
;
[org 0x7c00]          ; Tell the assembler where this code will be loaded.

mov bx, HELLO_MSG     ; Use bx as a parameter to our function, so we can 
call print_string     ; specify the address of the string.

mov bx, HEX_DMP
call print_string

mov dx, 0x1fb7 
call print_hex

mov bx, HEX_DMP2
call print_string

mov dx, 0xffff
call print_hex

mov ah, 0x0e
mov al, 10
int 0x10

jmp $



;%include "print_string.asm"
%include "print_hex.asm"


HELLO_MSG:
 db 'Hello World!', 0

HEX_DMP:
 db ' now printing 0x1FB7: ', 0

HEX_DMP2:
 db ' now printing: 0xffff: ', 0

; Padding and magic number.
times 510-($-$$) db 0
dw 0xaa55
