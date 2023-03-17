[org 0x00]
[bits 16]

section code

.init:
    ; mov es, 0xb800 Ideally we do this to move this value to the es register
    ; ES can't be directly written to, as it's an 'intermediate' register
    ; This is the beginning of video memory!
    mov eax, 0xb800
    mov es, eax
    mov eax, 0x00


.clear: ; When QEMU starts, our BIOS prints a lotta junk to video memory, lets clear it cuz gross!!
    mov byte [es:eax], 0
    inc eax
    mov byte [es:eax], 0x30
    inc eax

    cmp eax, 2 * 25 * 80
    jl .clear

.main:
    mov byte [es:0x00], 'H' ; move H to 0xb800 + 0x00
    mov byte [es:0x01], 0x30 ; color values


jmp $

times 510 - ($ - $$) db 0x00 ; Pads the file with 0s
db 0x55
db 0xAA
; Defines our magic flag for a bootable sector!