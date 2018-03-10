;
; print_hex prints 16-bit hex number to screen using BIOS.
;
; DEPENDENCIES
;  
; 	print_string.asm
;
; INPUT
;
; 	dx holds the hex value to be printed.
;
; Some ASCII values
;
; D   H   C
; 48  30  0    17 from 0-A
; 57  39  9
; 65  41  A
; 90  5A  Z
; 97  61  a
; 122 7A  z
%include "print_string.asm"

print_hex:
 pusha
 
 mov cx, 4           ; Counter - print 4 chars for a 16-bit sized word.

 char_loop:
 	dec cx

	mov ax, dx       ; We will use ax for comparisons.
	shr dx, 4        ; dx will continuously be shifted by one hex digit, hence cx.
	and ax, 0xf      ; mask ah to get last 4 bits (0xf gets the most significant part).

	mov bx, HEX_OUT 
	add bx, 2        ; skip the '0x' part.
	add bx, cx       ; modify string right to left.

	cmp ax, 0xa      ; check to see if it corresponds to a letter or a number.
	jl set_letter    ; If number then set the value, else, add 7.
	                 ; ASCII letters (A==65 or 41) start 17 chars after numbers
				     ; (9==57 or 39).
				     ; If our value is a letter, then it is already over 10 so 
					 ; we simply need 7 more.
	add byte [bx], 7
	jl set_letter


 set_letter:
 	add byte [bx], al ; Add the value of the byte to the char at bx.

	cmp cx, 0         
	je print_hex_done
	jmp char_loop


 print_hex_done:
 	mov bx, HEX_OUT
	call print_string
	call string_reset

	popa
	ret

;
; string_reset resets HEX_OUT for futher usegae.
;
string_reset:
 pusha

 mov bx, HEX_OUT
 add bx, 2

 start:
 	cmp byte [bx], 0    ; while not 0
	jne set             ; put 0 
	jmp finish

 set:
 	mov byte [bx], 0x30 ; 0 in ASCII
	inc bx
	jmp start

 finish:
 	popa
	ret


; Global vars
HEX_OUT:
 db '0x0000', 0
