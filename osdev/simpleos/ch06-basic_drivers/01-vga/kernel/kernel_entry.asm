;
; Jump into the kernel entrypoint function
;
[bits 32]
[extern main]
call main
jmp $
