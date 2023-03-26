#include "task.h"
#include "input.h"
#include "graphics_elements.h"

void ProcessTasks() {
    int priority = 0;

    while (priority <= 5)
    {
        for(int i = 0; i < TasksLength; i++) {
            if (tasks[i].priority == priority){
                tasks[i].function(tasks[i].taskId);
            }
        }
        priority++;
    }
}

int ClearScreenTask(int taskid) {
    ClearScreen(119.0f / 255.0f * 16.0f, 41.0f / 255.0f * 32.0f, 83.0f / 255.0f * 16.0f);

    return 0;
}

int DrawMouseTask(int taskid) {
    DrawMouse(mx,my, 0,0,0);

    return 0;
}

int HandleKeyboardTask(int taskid) {
    char* characterBuffer = tasks[taskid].ca1;
    int* characterBufferLength = &tasks[taskid].i1;
    char character = ProcessScancode(Scancode);

    if(backspace_pressed == TRUE) {
        characterBuffer[(*characterBufferLength) - 1] = '\0';
        (*characterBufferLength)--;
        backspace_pressed = FALSE;
        Scancode = -1;
    } else if (character != '\0' && character != -1) {
        characterBuffer[*characterBufferLength] = character;
        characterBuffer[*characterBufferLength + 1] = '\0';
        (*characterBufferLength)++;
        Scancode = -1;
    }

    DrawString(getArialCharacter, font_arial_width, font_arial_height, characterBuffer, 10, 10, 221, 72, 20);

    return 0;
}

int TestGraphicalElementsTask(int taskid) {
    if(left_clicked == FALSE) {
        iparams[taskid * task_params_length + 9] = FALSE;
    }

    /*
        iparams:
        0 - x
        1 - y
        2 - width
        3 - height
        9 - mouse click held down flag
    */
    if(
        iparams[taskid * task_params_length + 9] == TRUE || (
            (left_clicked == TRUE && mx > iparams[taskid * task_params_length + 0]) &&
            mx < iparams[taskid * task_params_length + 0] + iparams[taskid * task_params_length + 2] &&
            my > iparams[taskid * task_params_length + 1] &&
            my < iparams[taskid * task_params_length + 1] + 20
        )
    )
    {
        left_clicked = FALSE;
        iparams[taskid * task_params_length + 9] = TRUE;
        iparams[taskid * task_params_length + 0] = mx - (iparams[taskid*task_params_length+2]/2);
        iparams[taskid * task_params_length + 1] = my - 10;
    }

    DrawWindow(
            iparams[taskid * task_params_length + 0],
            iparams[taskid * task_params_length + 1],
            iparams[taskid * task_params_length + 2],
            iparams[taskid * task_params_length + 3], 221.0f / 255.0f * 16.0f, 72.0f / 255.0f * 32.0f, 20.0f / 255.0f * 16.0f
        );
    return 0;
}