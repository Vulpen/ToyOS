#include "task.h"
#include "input.h"

void ProcessTasks() {
    int priority = 0;

    while (priority <= 5)
    {
        for(int i = 0; i < TasksLength; i++) {
            if (tasks[i].priority == priority){
                if (tasks[i].type == task_type_void) {
                    tasks[i].function_void();
                } else if (tasks[i].type == task_type_string_buffer) {
                    tasks[i].function_string_buffer(tasks[i].param1, &tasks[i].param2);
                }
            }
        }
        priority++;
    }
}

int ClearScreenTask() {
    ClearScreen(119.0f / 255.0f * 16.0f, 41.0f / 255.0f * 32.0f, 83.0f / 255.0f * 16.0f);

    return 0;
}

int DrawMouseTask() {
    DrawMouse(x,y, 16,255,16);

    return 0;
}

int HandleKeyboardTask(char* characterBuffer, int* characterBufferLength) {
    char character = ProcessScancode(Scancode);

    if(backspace_pressed == TRUE) {
        characterBuffer[*characterBufferLength - 1] = '\0';
        characterBufferLength--;
        backspace_pressed = FALSE;
        Scancode = -1;
    } else if (character != '\0') {
        characterBuffer[*characterBufferLength] = character;
        characterBuffer[*characterBufferLength + 1] = '\0';
        (*characterBufferLength)++;
        Scancode = -1;
    }

    DrawString(getArialCharacter, font_arial_width, font_arial_height, characterBuffer, 10, 10, 221, 72, 20);

    return 0;
}