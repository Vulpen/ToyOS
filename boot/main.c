#include "graphics.h"

extern unsigned int isr1, isr12;
extern unsigned int base, base12;

int start() {
    // Cannot be more than 61 characters
    char str1 [] = "Welcome to Peepbuntu! \n ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    base = (unsigned int) &isr1;
    base12 = (unsigned int) &isr12;

    InitializeMouse();
    InitializeIDT();
    
    while(1) {
        ClearScreen(119.0f / 255.0f * 16.0f, 41.0f / 255.0f * 32.0f, 83.0f / 255.0f * 16.0f);
        DrawString(getArialCharacter, font_arial_width, font_arial_height, (char *)&str1, 0, 0, 221, 72, 20);

        DrawRect(x, y, 10, 10, 0, 0, 0);

        Flush();
    }
} 