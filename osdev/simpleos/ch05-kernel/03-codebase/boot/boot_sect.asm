;
; Boot sector to boot a C kernel in 32-bit protected mode
;
[org 0x7c00]

KERNEL_OFFSET equ 0x1000 ; This is the memory offset to which we will load our 
                         ; kernel.

mov [BOOT_DRIVE], dl     ; BIOS stores our boot drive in dl.

mov bp, 0x9000
mov sp, bp

mov bx, MSG_REAL_MODE
call print_string

call load_kernel

mov bx, KERNEL_LOADED
call print_string

call switch_to_pm

jmp $

;
; Inlcudes
;
%include "print/print_string.asm"
%include "print/print_string_pm.asm"
%include "disk/disk_load.asm"
%include "pm/gdt.asm"
%include "pm/switch_to_pm.asm"


[bits 16]
load_kernel:
	mov bx, MSG_LOAD_KERNEL
	call print_string

	mov bx, KERNEL_OFFSET   ; Set up parameters for the disk load routine. Load
	mov dh, 15              ; the first 15 sectors (excluding the boot sector)
	mov dl, [BOOT_DRIVE]    ; from the boot disk (our kernel code) to address 
	call disk_load          ; KERNEL_OFFSET.

	ret


[bits 32]
BEGIN_PM:
	mov ebx, MSG_PROT_MODE 
	call print_string_pm

	call KERNEL_OFFSET      ; Jump to address of our loaded kernel code.

	jmp $


;
; Global variables
;
BOOT_DRIVE      db 0
MSG_REAL_MODE   db "Starting 16-bit real mode...", 0
KERNEL_LOADED   db "Read kernel...", 0
MSG_PROT_MODE   db "Starting 32-bit protected mode...", 0
MSG_LOAD_KERNEL db "Loading kernel into memory...", 0

;
; Boot sector padding and magic number
;
times 510-($-$$) db 0
dw 0xaa55
