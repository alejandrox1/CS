; 
; print_string will print a string to screen using BIOS telescripting mode.
;
; INPUT:
;
; 	ebx should hold the address of the string to be printed.
;
[bits 32]

; Constants
VIDEO_MEMORY equ 0xb8000
WHITE_ON_BLACK equ 0x0f

;
; print_string_pm prints a null terminated string pointed to by edx.
;
print_string_pm:
 pusha

 mov edx, VIDEO_MEMORY

 print_string_pm_loop:
 	mov al, [ebx]
	mov ah, WHITE_ON_BLACK

	cmp al, 0
	je print_string_pm_done

	mov [edx], ax         ; Store char and attributes at current char cell.
	add edx, 2             ; Move to next char cell in vid mem.
	add ebx, 1             ; Increment to next char on screen.

	jmp print_string_pm_loop

 print_string_pm_done:
 	popa
 	ret
