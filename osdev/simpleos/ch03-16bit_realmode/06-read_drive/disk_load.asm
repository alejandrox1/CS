;
; Load N sectors to ES:BX from a given drive.
;
; INPUT
;
; 	dh number of sectors to be read.
; 	dl drive to read from.
;
; Registers used
;
; ah will hold the read opcode
; al needs the number of sectors to read from disk.
;
; ch select cylinder.
; cl sector to begin reading from.
; dh select a head/track (up or down, 0 | 1).
; dl drive to read from.
;
disk_load:
 push dx         ; Store dx to later we can recall how many sectors we 
                 ; requested to read.
 mov ah, 0x02    ; BIOS read sector function.
 mov al, dh      ; Number of sectors to read.

 mov ch, 0x00    ; Select cylinder 0.
 mov cl, 0x02    ; Start reading from second sector (i.e., after boot sector).  
                 ; 0x01 is the boot sector, 0x02 would then be free.            
 mov dh, 0x00    ; Select head 0 (0 | 1 for up or down).
 
 int 0x13        ; BIOS interrupt

 jc disk_error1  ; check if carry flag (cf) register was set. 

 pop dx
 cmp dh, al      ; al (secotrs read) cmp dh (sectors requested)
 jne disk_error2
 ret


 disk_error1:
 	mov bx, DISK_ERR_MSG
	call print_string
	jmp $

 disk_error2:
 	mov bx, INC_DISK_READ_MSG
	call print_string
	jmp $


DISK_ERR_MSG:
 db "Disk read error!", 0

INC_DISK_READ_MSG:
 db "Incomplete read fromdisk!", 0
