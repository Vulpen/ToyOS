#include "graphics.h"
#include "input.h"
#include "task.h"

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

    tasks[TasksLength].priority = 0;
    tasks[TasksLength].type = task_type_void;
    tasks[TasksLength].function_void = &ClearScreenTask;
    TasksLength++;

    tasks[TasksLength].priority = 0;
    tasks[TasksLength].type = task_type_void;
    tasks[TasksLength].function_void = &DrawMouseTask;
    TasksLength++;

    tasks[TasksLength].priority = 0;
    tasks[TasksLength].type = task_type_string_buffer;
    tasks[TasksLength].function_string_buffer = &HandleKeyboardTask;
    tasks[TasksLength].param2 = 0;
    TasksLength++;

    while(1) {

        ProcessTasks();

        Flush();
    }
}



