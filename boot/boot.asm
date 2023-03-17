[org 0x00]
[bits 16]

section code

.main:
    ; mov es, 0xb800 Ideally we do this to move this value to the es register
    ; ES can't be directly written to, as it's an 'intermediate' register
    ; This is the beginning of video memory!
    mov eax, 0xb800
    mov es, eax

    mov byte [es:0x00], 'H' ; move H to 0xb800 + 0x00
    mov byte [es:0x01], 0x30 ; color values

jmp $

times 510 - ($ - $$) db 0x00 ; Pads the file with 0s
db 0x55
db 0xAA
; Defines our magic flag for a bootable sector!