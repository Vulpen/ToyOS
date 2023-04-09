[org 0x7c00]
[bits 16]

section code

; Added this section for booting on real hardware from another course :fingers_crossed:
_start:
    jmp short .switch
    nop

; BIOS parameter block
times 33 db 0

; .init:
;     ; mov es, 0xb800 Ideally we do this to move this value to the es register
;     ; ES can't be directly written to, as it's an 'intermediate' register
;     ; This is the beginning of video memory!
;     mov eax, 0xb800
;     mov es, eax
;     mov eax, 0
;     mov ebx, 0  ; Index of the character in string
;     mov ecx, 0  ; Actual address of the character on screen
;     mov dl, 0   ; Store actual value of what we are printing

; .clear: ; When QEMU starts, our BIOS prints a lotta junk to video memory, lets clear it cuz gross!!
;     mov byte [es:eax], 0
;     inc eax
;     mov byte [es:eax], 0x02
;     inc eax

;     cmp eax, 2 * 25 * 80
;     jl .clear

;     mov eax, welcome
;     mov ecx, 0*2*80
;     call .print
;     jmp .switch

; .end:
;     ;mov byte [es:0x00], 'L' ; Debug Character
;     jmp $

.print:
    mov dl, byte [eax + ebx]

    cmp dl, 0
    je .print_end

    mov byte [es:ecx], dl
    inc ebx
    inc ecx
    inc ecx

    jmp .print

.print_end:
    mov eax, 0
    ret

.switch:
    ; mov eax, 0xb800       Attempt to add a print early on...
    ; mov bx, 0
    ; mov es, eax
    ; mov ax, welcome
    ; mov cx, 0*2*80
    ; call .print
    ; mov eax, 0
    ; mov es, eax

    mov ax, 0x4f01      ; querying the VBE
    mov cx, 0x111       ; VBE mode we want (resolution and other settings)
    mov bx, 0x0800      ; Offset for VBE info structure
    mov es, bx
    mov di, 0x00
    int 0x10             ; Graphics interrupt

    ; Make the switch to graphics mode
    mov ax, 0x4f02
    mov bx, 0x111
    int 0x10

    xor ax, ax
    mov ds, ax
    mov es, ax


    ; Load in our C code
    mov bx, 0x1000      ; Location of where the code is loaded from HDD
    mov ah, 0x02
    mov al, 37          ; Number of sectors to read, on a real device change this to 1
    mov ch, 0x00
    mov dh, 0x00
    mov cl, 0x02
    int 0x13

    cli     ; Disable interrupts
    lgdt [gdt_descriptor]   ; Load GDT Table

    mov eax, cr0
    or eax, 0x1
    mov cr0, eax    ; Switch to protected mode

    jmp code_seg:protected_start        ; Not sure why we need this offset

welcome: db 'Welcome to PeepOS!', 0

[bits 32]
protected_start:
   
    mov ax, data_seg
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Update stack pointer
    mov ebp, 0x90000
    mov esp, ebp

    call 0x1000         ; Load into our C code!
    jmp $

gdt_begin:
gdt_null_descriptor:
    dd 0x00
    dd 0x00
gdt_code_seg:
    dw 0xeeee
    dw 0x00
    db 0x00
    db 10011010b
    db 11001111b
    db 0x00
gdt_data_seg:
    dw 0xeeee
    dw 0x00
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00
gdt_end:
gdt_descriptor:
    dw gdt_end - gdt_begin - 1
    dd gdt_begin

code_seg equ gdt_code_seg - gdt_begin
data_seg equ gdt_data_seg - gdt_begin 

times 510 - ($ - $$) db 0x00 ; Pads the file with 0s
db 0x55
db 0xAA
; Defines our magic flag for a bootable sector!