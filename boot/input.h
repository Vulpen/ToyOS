#ifndef INPUT_H
#define INPUT_H

#include "graphics.h"
#include <stdint.h> 

#define pic1_command 0x20       // pic is the interrupt chip
#define pic1_data 0x21
#define pic2_command 0xa0
#define pic2_data 0xa1
#define icw1_def 0x10
#define icw1_icw4 0x01
#define icw4_x86 0x01

#define y_overflow      0b10000000
#define x_overflow      0b01000000
#define y_negative      0b00100000
#define x_negative      0b00010000
#define always_set      0b00001000
#define middle_click    0b00000100
#define right_click     0b00000010
#define left_click      0b00000001
#define unused_a        0b10000000
#define unused_b        0b01000000

int mx, my;
int left_clicked, right_clicked, middle_clicked;
int current_byte = 0;
uint8_t bytes[4] = { 0 };
int mouse_speed = 3;
int mouse_possessed_task_id = 0;

void MouseWait(unsigned char type);
void MouseWrite(unsigned char data);
unsigned char MouseRead();
void HandleMousePacket();
void HandleMouseInterrupt();
void InitializeMouse();

// These don't have a 'valid' input character, not sure what exactly that means?
int shift_pressed = FALSE;
int caps_pressed = FALSE;
int escape_pressed = FALSE;
int backspace_pressed = FALSE;
int alt_pressed = FALSE;
int ctrl_pressed = FALSE;
int enter_pressed = FALSE;

char ProcessScancode(int scancode);

void InitializeIDT();
extern void LoadIDT();
void HandleISR1();
void HandleISR12();
void RemapPIC();

int Scancode = -1;

struct IDTElement {
    unsigned short lower;
    unsigned short selector;
    unsigned char zero;
    unsigned char flags;
    unsigned short higher;
};

struct IDTElement _idt[256];
extern unsigned int isr1, isr12;        // Assembly functions
unsigned int base, base12;

#endif