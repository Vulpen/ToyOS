#include "graphics.h"
#include "input.h"

// extern unsigned int isr1, isr12;
// extern unsigned int base, base12;
// extern int x, y;
// extern int Scancode;

// extern int shift_pressed;
// extern int caps_pressed;
// extern int escape_pressed;
// extern int backspace_pressed;
// extern int alt_pressed;
// extern int ctrl_pressed;
// extern int enter_pressed;

int start() {
    VBEInfoBlock* VBE = (VBEInfoBlock*) VBEInfoAddress;

    x = VBE->x_resolution / 2;
    y = VBE->y_resolution / 2;

    // Cannot be more than 61 characters
    char str1 [] = "Welcome to Peepbuntu! \n ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    char characterBuffer[1000] = "\0";              // Buffer for keyboard input
    char* characterBufferPointer = characterBuffer;
    int characterBufferLength = 0;

    base = (unsigned int) &isr1;
    base12 = (unsigned int) &isr12;

    InitializeMouse();
    InitializeIDT();
    
    while(1) {
        char character = ProcessScancode(Scancode);

        if(backspace_pressed == TRUE) {
            characterBuffer[characterBufferLength - 1] = '\0';
            characterBufferLength--;
            backspace_pressed = FALSE;
            Scancode = -1;
        } else if (character != '\0') {
            characterBuffer[characterBufferLength] = character;
            characterBuffer[characterBufferLength + 1] = '\0';
            characterBufferLength++;
            Scancode = -1;
        }

        ClearScreen(119.0f / 255.0f * 16.0f, 41.0f / 255.0f * 32.0f, 83.0f / 255.0f * 16.0f);
        //DrawString(getArialCharacter, font_arial_width, font_arial_height, (char *)&str1, 0, 0, 221, 72, 20);
        DrawString(getArialCharacter, font_arial_width, font_arial_height, characterBufferPointer, 0, 0, 221, 72, 20);

        DrawMouse(x,y, 16,255,16);

        Flush();
    }
}

