#include <stdint.h>         // Need to include this so we can guarantee some data sizes are actually a byte (uint8_
#include "input.h"



unsigned char inportb(unsigned short port) {
    unsigned char value;

    __asm__ __volatile__("inb %1, %0" : "=a" (value) : "dN" (port));

    return value;
}

void outportb(unsigned short port, unsigned char data) {
    __asm__ __volatile__("outb %1, %0" : : "dN" (port), "a" (data));
}

void InitializeIDT() {
    _idt[1].lower = (base & 0xffff);
    _idt[1].higher = (base >> 16) & 0xffff;
    _idt[1].selector = 0x08;
    _idt[1].zero = 0;
    _idt[1].flags = 0x8e;

    _idt[12].lower = (base12 & 0xffff);
    _idt[12].higher = (base12 >> 16) & 0xffff;
    _idt[12].selector = 0x08;
    _idt[12].zero = 0;
    _idt[12].flags = 0x8e;

    RemapPIC();

    outportb(0x21, 0b11111001);     // Unmask mouse and keyboard
    outportb(0xa1, 0x00);

    LoadIDT();
}

void RemapPIC() {
    unsigned char a, b;

    a = inportb(pic1_data);
    b = inportb(pic1_data);

    outportb(pic1_command, icw1_def | icw1_icw4);
    outportb(pic2_command, icw1_def | icw1_icw4);

    outportb(pic1_data, 0);
    outportb(pic2_data, 8);

    outportb(pic1_data, 4);
    outportb(pic1_data, 2);

    outportb(pic1_data, icw4_x86);
    outportb(pic1_data, icw4_x86);

    outportb(pic1_data, a);
    outportb(pic2_data, b);
}

void HandleISR1() {
    Scancode = inportb(0x60);           // Keyboard input read
    outportb(0xa0, 0x20);
    outportb(0x20, 0x20);
}

void MouseWait(unsigned char type) {
    int time_out = 10000;

    if (type == 0) {
        while (time_out--) {
            if((inportb(0x64) & 1) == 1)
                return;
        }
        return;
    } else {
        while (time_out--) {
            if ((inportb(0x64) & 2) == 0)
                return;
        }
        return;
    }
}

void MouseWrite(unsigned char data) {
    MouseWait(1);
    outportb(0x64, 0xd4);

    MouseWait(1);
    outportb(0x60, data);
}

unsigned char MouseRead() {
    MouseWait(0);
    return inportb(0x60);
}


void HandleISR12() {
    HandleMouseInterrupt();
    outportb(0xa0, 0x20);
    outportb(0x20, 0x20);
}

void HandleMouseInterrupt() {
    uint8_t byte = MouseRead();

    // if always set isn't set, it's junk and lets toss it
    if (current_byte == 0 && !(byte & always_set)) 
        return;

    bytes[current_byte] = byte;
    current_byte++;

    if (current_byte >= 3)
        current_byte = 0;
    
    if(current_byte == 0)
        HandleMousePacket();
}

void HandleMousePacket() {
    VBEInfoBlock* VBE = (VBEInfoBlock*) VBEInfoAddress;

    uint8_t status = bytes[0];
    int32_t change_x = (int32_t)bytes[1];
    int32_t change_y = (int32_t)bytes[2];

    // If we're overflowing, likely garbage data
    if (status & x_overflow || status & y_overflow)
        return;

    if (status & x_negative)
        change_x |= 0xffffff00;

    if (status & y_negative)
        change_y |= 0xffffff00;

    left_clicked = status & left_click;
    right_clicked = status & right_click;
    middle_clicked = status & middle_click;

    if (change_x > 0) {
        mx += 6;
    } else if (change_x < 0) {
        mx -= 6;
    }

    if (change_y > 0) {
        my -= 6;
    } else if (change_y < 0) {
        my += 6;
    }

    if( mx < 0) {
        mx = 0;
    } else if (mx > VBE->x_resolution) {
        mx = VBE->x_resolution - 15;
    }

    if(my < 0) {
        my = 0;
    } else if (my > VBE->y_resolution) {
        my = VBE->y_resolution - 15;
    }
}

void InitializeMouse() {
    unsigned char status;

    MouseWait(1);
    outportb(0x64, 0xd4);

    MouseWait(1);
    outportb(0x64, 0xa8);

    MouseWait(1);
    outportb(0x64, 0x20);
    MouseWait(0);
    status = (inportb(0x60) | 2);
    MouseWait(1);
    outportb(0x64, 0x60);
    MouseWait(1);
    outportb(0x60, status);

    MouseWrite(0xff);
    MouseRead();
    MouseWrite(0xf6);
    MouseRead;

    MouseWrite(0xf4);
    MouseRead();
}

// This seems horrible, def look for a way to fix
char ProcessScancode(int scancode) {
    // No idea why, but this function returns unsigned char, yet the course has us returning -1.
    // His compiler seems totally fine with that

    if (scancode ==  0x01)
        escape_pressed = TRUE;
        
    else if (scancode ==  0x02)
        if (shift_pressed == TRUE)
            return '!';
        else 
            return '1';
        
    else if (scancode ==  0x03)
        if (shift_pressed == TRUE)
            return '"';
        else 
            return '2';
        
    else if (scancode ==  0x04)
        if (shift_pressed == TRUE)
            return '£';
        else 
            return '3';
        
    else if (scancode ==  0x05)
        if (shift_pressed == TRUE)
            return '$';
        else 
            return '4';
            
    else if (scancode == 0x06)
        if (shift_pressed == TRUE)
            return '%';
        else 
            return '5';
        
    else if (scancode == 0x07)
        if (shift_pressed == TRUE)
            return '^';
        else 
            return '6';
        
    else if (scancode == 0x08)
        if (shift_pressed == TRUE)
            return '&';
        else 
            return '7';
        
    else if (scancode == 0x09)
        if (shift_pressed == TRUE)
            return '*';
        else 
            return '8';
        
    else if (scancode == 0x0A)
        if (shift_pressed == TRUE)
            return '(';
        else 
            return '9';
        
    else if (scancode == 0x0B)
        if (shift_pressed == TRUE)
            return ')';
        else 
            return '0';
        
    else if (scancode == 0x0C)
        if (shift_pressed == TRUE)
            return '_';
        else 
            return '-';
            
    else if (scancode == 0x0D)
        if (shift_pressed == TRUE)
            return '+';
        else 
            return '=';
        
    // Backspace
    else if (scancode == 0x0E) {
        backspace_pressed = TRUE;
    }
       
        
    else if (scancode == 0x0F)
        return '\t';
        
    else if (scancode == 0x10)
        if (shift_pressed == TRUE || caps_pressed == TRUE)
            return 'Q';
        else 
            return 'q';
        
    else if (scancode == 0x11)
        if (shift_pressed == TRUE || caps_pressed == TRUE)
            return 'W';
        else 
            return 'w';
        
    else if (scancode == 0x12)
        if (shift_pressed == TRUE || caps_pressed == TRUE)
            return 'E';
        else 
            return 'e';
        
    else if (scancode == 0x13)
        if (shift_pressed == TRUE || caps_pressed == TRUE)
            return 'R';
        else 
            return 'r';
        
    else if (scancode == 0x14)
        if (shift_pressed == TRUE || caps_pressed == TRUE)
            return 'T';
        else 
            return 't';
        
    else if (scancode == 0x15)
        if (shift_pressed == TRUE || caps_pressed == TRUE)
            return 'Y';
        else 
            return 'y';
        
    else if (scancode == 0x16)
        if (shift_pressed == TRUE || caps_pressed == TRUE)
            return 'U';
        else 
            return 'u';
        
    else if (scancode == 0x17)
        if (shift_pressed == TRUE || caps_pressed == TRUE)
            return 'I';
        else 
            return 'i';
        
    else if (scancode == 0x18)
        if (shift_pressed == TRUE || caps_pressed == TRUE)
            return 'O';
        else 
            return 'o';
        
    else if (scancode == 0x19)
        if (shift_pressed == TRUE || caps_pressed == TRUE)
            return 'P';
        else 
            return 'p';
        
    else if (scancode == 0x1A)
        if (shift_pressed == TRUE)
            return '{';
        else 
            return '[';
        
    else if (scancode == 0x1B)
        if (shift_pressed == TRUE)
            return '}';
        else 
            return ']';
        
    // enter pressed
    else if (scancode == 0x1C) {
        enter_pressed = TRUE;
        return '\n';
    }
        
        
    // ctrl pressed
    else if (scancode == 0x1D)
        ctrl_pressed = TRUE;
        
        
    else if (scancode == 0x1E)
        if (shift_pressed == TRUE || caps_pressed == TRUE)
            return 'A';
        else 
            return 'a';
        
    else if (scancode == 0x1F)
        if (shift_pressed == TRUE || caps_pressed == TRUE)
            return 'S';
        else 
            return 's';
        
    else if (scancode == 0x20)
        if (shift_pressed == TRUE || caps_pressed == TRUE)
            return 'D';
        else 
            return 'd';
        
    else if (scancode == 0x21)
        if (shift_pressed == TRUE || caps_pressed == TRUE)
            return 'F';
        else 
            return 'f';
        
    else if (scancode == 0x22)
        if (shift_pressed == TRUE || caps_pressed == TRUE)
            return 'G';
        else 
            return 'g';
        
    else if (scancode == 0x23)
        if (shift_pressed == TRUE || caps_pressed == TRUE)
            return 'H';
        else 
            return 'h';
        
    else if (scancode == 0x24)
        if (shift_pressed == TRUE || caps_pressed == TRUE)
            return 'J';
        else 
            return 'j';
        
    else if (scancode == 0x25)
        if (shift_pressed == TRUE || caps_pressed == TRUE)
            return 'K';
        else 
            return 'k';
        
    else if (scancode == 0x26)
        if (shift_pressed == TRUE || caps_pressed == TRUE)
            return 'L';
        else 
            return 'l';
        
    else if (scancode == 0x27)
        if (shift_pressed == TRUE)
            return ':';
        else 
            return ';';
        
    else if (scancode == 0x28) 
        if (shift_pressed == TRUE)
            return '@';
        else 
            return '\'';
        
    else if (scancode == 0x29)
        if (shift_pressed == TRUE)
            return '¬';
        else 
            return '`';
        
    // shift pressed
    else if (scancode == 0x2A) {
        shift_pressed = TRUE;
        return -1;
    }
        
        
        
    else if (scancode == 0x2B)
        if (shift_pressed == TRUE)
            return '|';
        else 
            return '\\';
    
    else if (scancode == 0x2C)
        if (shift_pressed == TRUE || caps_pressed == TRUE)
            return 'Z';
        else 
            return 'z';
    
    else if (scancode == 0x2D)
        if (shift_pressed == TRUE || caps_pressed == TRUE)
            return 'X';
        else 
            return 'x';
        
    else if (scancode == 0x2E)
        if (shift_pressed == TRUE || caps_pressed == TRUE)
            return 'C';
        else 
            return 'c';
        
    else if (scancode == 0x2F)
        if (shift_pressed == TRUE || caps_pressed == TRUE)
            return 'V';
        else 
            return 'v';
        
    else if (scancode == 0x30)
        if (shift_pressed == TRUE || caps_pressed == TRUE)
            return 'B';
        else 
            return 'b';
        
    else if (scancode == 0x31)
        if (shift_pressed == TRUE || caps_pressed == TRUE)
            return 'N';
        else 
            return 'n';
        
    else if (scancode == 0x32)
        if (shift_pressed == TRUE || caps_pressed == TRUE)
            return 'M';
        else 
            return 'm';
        
    else if (scancode == 0x33)
        if (shift_pressed == TRUE)
            return '<';
        else 
            return ',';
        
    else if (scancode == 0x34)
        if (shift_pressed == TRUE)
            return '>';
        else 
            return '.';
    
    else if (scancode == 0x35)
        if (shift_pressed == TRUE)
            return '?';
        else 
            return '/';

    // shift pressed    
    else if (scancode == 0x36) {
        shift_pressed = TRUE;
        return -1;
    }

        

    // alt pressed
    else if (scancode == 0x38) {
        alt_pressed = TRUE;
    }
        
        
    else if (scancode == 0x39)
        return ' ';
    
    // Caps pressed
    else if (scancode == 0x3A) {
        if (caps_pressed == TRUE)
            caps_pressed = FALSE;
        else if (caps_pressed == FALSE)
            caps_pressed = TRUE;
        return -1;
    }
        

    // shift released
    if (scancode == 0xAA) {
        shift_pressed = FALSE;
        return -1;
    }
        

    // shift released
    if (scancode == 0xB6)  {
        shift_pressed = FALSE;
        return -1;
    }
    return '\0';
}