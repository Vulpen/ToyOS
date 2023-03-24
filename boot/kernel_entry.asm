[bits 32]
START:
[extern start]
    call start
    jmp $

; Get access to our C code for input!
extern _idt, HandleISR1, HandleISR12
global isr1, isr12
global LoadIDT

IDTDesc:
    dw 2048
    dd _idt

; So these are super neat, we're defined our interrupts and having them call functions defined in our C code!
isr1:
    pusha
    call HandleISR1
    popa
    iret

isr12:
    pusha
    call HandleISR12
    popa
    iret

LoadIDT:
    lidt[IDTDesc]
    sti
    ret