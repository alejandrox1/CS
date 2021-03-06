;
; idt.s 
;
; Set up IDT.
; Taken from JamesM' kernel development tutorials.
;
[GLOBAL idt_flush]      ; Allow kernel to make call.

idt_flush:
    mov eax, [esp+4]    ; Get pointer to IDT (passed as func parameter).
    lidt [eax]
	ret


;
; Macro creates a stub for an ISR which does not pass it's own error code (adds 
; a dummy errro code byte).
;
%macro ISR_NOERRCODE 1
    global isr%1
	isr%1:
        cli
		push byte 0     ; Push dummy error code.
		push %1         ; Push the interrupt number.
		jmp isr_common_stub 
%endmacro

;
; Macro creates a stub for an ISR which passes it's own error code.
;
%macro ISR_ERRCODE 1
    global isr%1
	isr%1:
        cli
		push %1    ; Push the interrupt number.
		jmp isr_common_stub
%endmacro

;
; Macro creates a stub for an IRQ.
; The first parameter is the IRQ number, the second is the ISR number it is 
; remapped to.
%macro IRQ 2
    global irq%1
    irq%1:
        cli
        push byte 0
        push byte %2
        jmp irq_common_stub
%endmacro
    

ISR_NOERRCODE 0                                                                 
ISR_NOERRCODE 1                                                                 
ISR_NOERRCODE 2                                                                 
ISR_NOERRCODE 3                                                                 
ISR_NOERRCODE 4                                                                 
ISR_NOERRCODE 5                                                                 
ISR_NOERRCODE 6                                                                 
ISR_NOERRCODE 7                                                                 
ISR_ERRCODE   8                                                                 
ISR_NOERRCODE 9                                                                 
ISR_ERRCODE   10                                                                
ISR_ERRCODE   11                                                                
ISR_ERRCODE   12                                                                
ISR_ERRCODE   13                                                                
ISR_ERRCODE   14                                                                
ISR_NOERRCODE 15                                                                
ISR_NOERRCODE 16                                                                
ISR_NOERRCODE 17                                                                
ISR_NOERRCODE 18                                                                
ISR_NOERRCODE 19                                                                
ISR_NOERRCODE 20                                                                
ISR_NOERRCODE 21                                                                
ISR_NOERRCODE 22                                                                
ISR_NOERRCODE 23                                                                
ISR_NOERRCODE 24                                                                
ISR_NOERRCODE 25                                                                
ISR_NOERRCODE 26                                                                
ISR_NOERRCODE 27                                                                
ISR_NOERRCODE 28                                                                
ISR_NOERRCODE 29                                                                
ISR_NOERRCODE 30                                                                
ISR_NOERRCODE 31                                                                
ISR_NOERRCODE 255
IRQ   0,    32                                                                  
IRQ   1,    33                                                                  
IRQ   2,    34                                                                  
IRQ   3,    35                                                                  
IRQ   4,    36                                                                  
IRQ   5,    37                                                                  
IRQ   6,    38                                                                  
IRQ   7,    39                                                                  
IRQ   8,    40                                                                  
IRQ   9,    41                                                                  
IRQ  10,    42                                                                  
IRQ  11,    43                                                                  
IRQ  12,    44                                                                  
IRQ  13,    45                                                                  
IRQ  14,    46                                                                  
IRQ  15,    47 


;
; extern functions
;
extern idt_handler
extern irq_handler

;
; isr_common_stub saves the processor's state, sets up the kernel mode 
; segments, calls the C-level fault handler, and finally restores the stack 
; frame.
;
isr_common_stub:
    pusha        ; Save edi, esi, ebp, esp, eax, ebx, ecx, edx.
	mov ax, ds   ; Lower 16 bits of data segement register.
	push eax     ; Save data segement descriptor in stack.

	mov ax, 0x10 ; Load the kernel data segment descriptor.
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	         ; Push a pointer to the current top of the stack. This 
	                 ; will become the register_t* parameter.
	call idt_handler ; C routine (idt.c).
	       ; Remove the pointer to the stack from the stack (ha!).

	pop ebx          ; Load data segment descriptor.
	mov ds, bx
	mov es, bx
	mov fs, bx
	mov gs, bx

	popa
	add esp, 8       ; Clean up the pushed error code and ISR number.
	sti
    iret             ; pop CS, SS, ESP, EIP, and EFLAGS.


;
; irq_common_stub saves the processor state, sets up the kernel mode segments,
; calls the C-level fault handler, and finally restores the stack frame.
;
irq_common_stub:
    pusha
    mov ax, ds
    push eax

    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    call irq_handler

    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa
    add esp, 8 
    sti
    iret
