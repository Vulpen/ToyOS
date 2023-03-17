[org 0x00]
[bits 16]

section code

.init:
    mov eax, 0x07c0    ; Prepare data segment
    mov ds, eax 
    ; mov es, 0xb800 Ideally we do this to move this value to the es register
    ; ES can't be directly written to, as it's an 'intermediate' register
    ; This is the beginning of video memory!
    mov eax, 0xb800
    mov es, eax
    mov eax, 0
    mov ebx, 0  ; Index of the character in string
    mov ecx, 0  ; Actual address of the character on screen
    mov dl, 0   ; Store actual value of what we are printing

.clear: ; When QEMU starts, our BIOS prints a lotta junk to video memory, lets clear it cuz gross!!
    mov byte [es:eax], 0
    inc eax
    mov byte [es:eax], 0x02
    inc eax

    cmp eax, 2 * 25 * 80
    jl .clear

    mov eax, text
    mov ecx, 3*2*80 + 0x02
    call .print
    ;push .end   ; Push the next location for 'ret' to return to

.end:
    ;mov byte [es:0x00], 'L' ; Debug Character
    jmp $

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

text: db 'Hello, World!', 0
text1: db 'This is another text.', 0

times 510 - ($ - $$) db 0x00 ; Pads the file with 0s
db 0x55
db 0xAA
; Defines our magic flag for a bootable sector!