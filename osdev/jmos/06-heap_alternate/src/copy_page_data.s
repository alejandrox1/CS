; [EXTERN monitor_write]
; [EXTERN monitor_write_hex]

; msg1:
;   db "Inside copy page data! src: ", 0
; msg2:
;   db " dst: " , 0
; msg3:
;   db 10, 0


[GLOBAL copy_page_data]
copy_page_data:
  
  push ebx            ; __cdecl requires the contents of ebx be preserved
  pushf               ; this makes possible preserving the contents of eflags
  cli                 ; disable interrupts

  ; mov  ebx, msg1
  ; push ebx
  ; call monitor_write
  ; pop  ebx
  
  mov ebx, [esp+12]   ; source address
  mov ecx, [esp+16]   ; destination address


  ; push ecx
  ; push ebx
  ; call monitor_write_hex
  ; mov  ebx, msg2
  ; push ebx
  ; call monitor_write
  ; pop  ebx
  ; pop  ebx
  ; call monitor_write_hex
  ; mov  ecx, msg3
  ; push ecx
  ; call monitor_write
  ; pop  ecx
  ; pop  ecx  
  
  mov edx, cr0        ; clear bit 31 in cr0
  and edx, 0x7fffffff ; to disable paging
  mov cr0, edx
  
  mov edx, 1024       ; 1024 * 4 bytes = 4096 = 0x1000 bytes to copy
.loop:
  mov eax, [ebx]      ; copy
  mov [ecx], eax      ; word
  add ebx, 4          ; by word
  add ecx, 4          ; the contents of source
  dec edx             ; to destination
  jnz .loop
  
  mov edx, cr0        
  or  edx, 0x80000000 ; set bit 31 in cr0 (control register 0)
  mov cr0, edx        ; to enable paging
  
  popf                ; restore eflags
  pop ebx             ; ebx and
  ret                 ; return


; CONTENTS OF STACK at line 8
; esp +16 *** esp+19 : destination address
; esp +12 *** esp+15 : source address
; esp + 8 *** esp+11 : return address
; esp + 4 *** esp+ 7 : value of ebx
; esp + 0 *** esp+ 3 : value of flags

 
