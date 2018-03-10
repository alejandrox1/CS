;
; Boot sector going into 32-bit protected mode.
;
[org 0x7c00]

mov bp, 0x9000
mov sp, bp

mov bx, MSG_REAL_MODE
call print_string

call switch_to_pm     ; Never return from here.

jmp $



%include "print_string.asm"
%include "print_string_pm.asm"
%include "gdt.asm"
%include "switch_to_pm.asm"

[bits 32]
;
; This is where we arrive after switching to initializing protected mode.
;
BEGIN_PM:
 mov ebx, MSG_PROT_MODE
 call print_string_pm

 jmp $



;
; Global variables
;
MSG_REAL_MODE db "Starting 16-bit Real Mode... ", 0

MSG_PROT_MODE db "Starting 32-bit Protected Mode", 0

;
; Boot sector padding and magic number.
;
times 510-($-$$) db 0
dw 0xaa55
