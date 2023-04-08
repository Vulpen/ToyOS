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

void CloseTask(int taskid) {
    for(int i = taskid; i < TasksLength-1; i++) {
        tasks[i] = tasks[i+1];
    }

    TasksLength--;
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
    int* r = &iparams[taskid * task_params_length + 4];
    int* g = &iparams[taskid * task_params_length + 5];
    int* b = &iparams[taskid * task_params_length + 6];

    if (
        DrawWindow(
            &iparams[taskid * task_params_length + 0],
            &iparams[taskid * task_params_length + 1],
            &iparams[taskid * task_params_length + 2],
            &iparams[taskid * task_params_length + 3],
            iparams[taskid * task_params_length + 4],
            iparams[taskid * task_params_length + 5],
            iparams[taskid * task_params_length + 6],
            &iparams[taskid * task_params_length + 9]
        ) == 1
    ) {
        CloseTask(taskid);
    }

    char text[] = "Dark\0";
    char text1[] = "Light\0";

    if (
        DrawButton(
            iparams[taskid * task_params_length + 0] + 20,
            iparams[taskid * task_params_length + 1] + 20,
            50, 20, 0, 32, 0,
            text, 16, 32, 16)
        == TRUE)  {
        *r = 0; *g = 0; *b = 0;
    }

    if (
        DrawButton(
            iparams[taskid * task_params_length + 0] + 100,
            iparams[taskid * task_params_length + 1] + 20,
            50, 20, 0, 32, 0,
            text1, 16, 32, 16)
        == TRUE)  {
        *r = 221.0f / 255.0f * 16.0f;
        *g = 72.0f / 255.0f * 32.0f;
        *b = 20.0f / 255.0f * 16.0f;
    }

    return 0;
}