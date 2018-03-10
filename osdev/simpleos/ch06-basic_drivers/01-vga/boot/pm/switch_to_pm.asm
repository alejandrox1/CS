[bits 16]
; Switch to protected mode.
switch_to_pm:
 cli                   ; Switch of interrupts until we have set-up the 
                       ; protected mode 
                       ; interrupt vector.
 
 lgdt [gdt_descriptor] ; Load our global descriptor table, which defines the 
                       ; protected mode segments.
 
 mov eax, cr0          ; To make the switch to protected mode, set the first 
 or eax, 0x1           ; bit of cr0, a control register.
 mov cr0, eax

 jmp CODE_SEG:init_pm  ; Make a far jump to force the cpu to flush its cache of 
                       ; pre-fetched and real-mode ecoded instructions.



[bits 32]
; Initialize registers and stack at once in PM.
init_pm:
 mov ax, DATA_SEG      
 mov ds, ax
 mov ss, ax
 mov es, ax
 mov fs, ax
 mov gs, ax

 mov ebp, 0x90000      ; Put stack a top of the free space.
 mov esp, ebp

 call BEGIN_PM         ; Call a well known label.
