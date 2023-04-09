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

    mx = VBE->x_resolution / 2;
    my = VBE->y_resolution / 2;

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
    tasks[TasksLength].function = &ClearScreenTask;
    TasksLength++;

    tasks[TasksLength].priority = 0;
    tasks[TasksLength].taskId = TasksLength;
    tasks[TasksLength].function = &TestGraphicalElementsTask;
    iparams[TasksLength * task_params_length + 0] = 10;
    iparams[TasksLength * task_params_length + 1] = 30;
    iparams[TasksLength * task_params_length + 2] = 300;
    iparams[TasksLength * task_params_length + 3] = 300;
    iparams[TasksLength * task_params_length + 4] = 0;
    iparams[TasksLength * task_params_length + 5] = 0;
    iparams[TasksLength * task_params_length + 6] = 0;
    TasksLength++;

    tasks[TasksLength].priority = 0;
    tasks[TasksLength].taskId = TasksLength;
    tasks[TasksLength].function = &TestGraphicalElementsTask;
    iparams[TasksLength * task_params_length + 0] = 50;
    iparams[TasksLength * task_params_length + 1] = 50;
    iparams[TasksLength * task_params_length + 2] = 300;
    iparams[TasksLength * task_params_length + 3] = 300;
    iparams[TasksLength * task_params_length + 4] = 0;
    iparams[TasksLength * task_params_length + 5] = 0;
    iparams[TasksLength * task_params_length + 6] = 0;
    TasksLength++;

    tasks[TasksLength].priority = 0;
    tasks[TasksLength].function = &HandleKeyboardTask;
    //tasks[TasksLength].i1 = 0;
    TasksLength++;

    tasks[TasksLength].priority = 5;
    tasks[TasksLength].function = &DrawMouseTask;
    TasksLength++;


    while(1) {

        ProcessTasks();

        Flush();
    }
}



