;
; A boot sector that prints "Hello World."
;
[org 0x7c00]          ; Tell the assembler where this code will be loaded.

mov bx, HELLO_MSG     ; Use bx as a parameter to our function, so we can 
call print_string     ; specify the address of the string.

mov ah, 0x0e
mov al, 10
int 0x10

jmp $



%include "print_string.asm"

HELLO_MSG:
 db 'Hello World!', 0



; Padding and magic number.
times 510-($-$$) db 0
dw 0xaa55
