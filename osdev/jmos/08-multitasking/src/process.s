[GLOBAL read_eip]
read_eip:
    pop eax    ; When read_eip the current isntruction is pushed onto the stack.
    jmp eax    ; Here, we pop the return value into EAX ourselves.

[GLOBAL perform_task_switch]
perform_task_switch:
     cli;
     mov ecx, [esp+4]   ; EIP
     mov eax, [esp+8]   ; physical address of current directory
     mov ebp, [esp+12]  ; EBP
     mov esp, [esp+16]  ; ESP
     mov cr3, eax       ; set the page directory
     mov eax, 0x12345   ; magic number to detect a task switch
     sti;
     jmp ecx

[GLOBAL copy_page_physical]
copy_page_physical:
    push ebx             ; Acording to __cdecl, we must preserve the content of EBX.
    pushf                ; Push EFLAGS, so we can pop it an re-enable interrupts 
                         ; later, if they were enabled anyway.
    cli                  ; Disable interrupts, we don't want to be interrupted.
                         ; Load these in before we disable paging.
    mov ebx, [esp+12]    ; Source address.
    mov ecx, [esp+16]    ; Destination address.

    mov edx, cr0         ; Get control register to disable paging.
    and edx, 0x7fffffff
    mov cr0, edx

    mov edx, 1024

.loop:
    mov eax, [ebx]       ; Get word at source address.
    mov [ecx], eax       ; Store it at the destination address.
    add ebx, 4           ; Source address += sizeof(word)
    add ecx, 4           ; Dest address += sizeof(word)
    dec edx              ; One less word to do.
    jnz .loop            

    mov edx, cr0         ; Get control register and enable paging.
    or  edx, 0x80000000
    mov cr0, edx

    popf
    pop ebx
    ret
