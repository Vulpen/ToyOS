#include "task.h"
#include "input.h"
#include "graphics_elements.h"

void ProcessTasks() {
    int priority;

    priority = 5;
    int i = 0;
    while(priority >= 0) {
        // Set mouse possessed taskid to the highest priority.
        // Notice this assumes that variables are passed in a certain order unfortunately.
        i = mouse_possessed_task_id;
        if (
                left_clicked == TRUE &&
                mx > iparams[i * task_params_length + 0] &&
                mx < iparams[i * task_params_length + 0] + iparams[i * task_params_length + 2] &&
                my > iparams[i * task_params_length + 1] &&
                my < iparams[i * task_params_length + 1] + iparams[i * task_params_length + 3]
                ){
                break;
            }

        for(i = 0; i < TasksLength; i++) {
            if (
                left_clicked == TRUE &&
                mx > iparams[i * task_params_length + 0] &&
                mx < iparams[i * task_params_length + 0] + iparams[i * task_params_length + 2] &&
                my > iparams[i * task_params_length + 1] &&
                my < iparams[i * task_params_length + 1] + iparams[i * task_params_length + 3]
                ){
                    tasks[mouse_possessed_task_id].priority = 0;
                    mouse_possessed_task_id = i;
                    tasks[i].priority = 2;
                    left_clicked = FALSE;
            }
        }
        priority--;
    }

    priority = 0;
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

void NullTask(int taskId) {
    return 0;
}

void CloseTask(int taskid) {
    tasks[taskid].function = &NullTask;
    iparams[TasksLength * task_params_length + 0] = 0;
    iparams[TasksLength * task_params_length + 1] = 0;
    iparams[TasksLength * task_params_length + 2] = 0;
    iparams[TasksLength * task_params_length + 3] = 0;
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

   int closed_clicked = DrawWindow(
            &iparams[taskid * task_params_length + 0],
            &iparams[taskid * task_params_length + 1],
            &iparams[taskid * task_params_length + 2],
            &iparams[taskid * task_params_length + 3],
            iparams[taskid * task_params_length + 4],
            iparams[taskid * task_params_length + 5],
            iparams[taskid * task_params_length + 6],
            &iparams[taskid * task_params_length + 9],
            taskid
        );

    int x  = iparams[taskid * task_params_length + 0];
    int y  = iparams[taskid * task_params_length + 1];
    int width  = iparams[taskid * task_params_length + 2];
    int height  = iparams[taskid * task_params_length + 3];

    if(closed_clicked == TRUE) {
        CloseTask(taskid);
    }

    char text[] = "Dark\0";
    char text1[] = "Light\0";

    if (
        DrawButton(
            x + 20,
            y + 20,
            50, 20, 0, 32, 0,
            text, 16, 32, 16, taskid)
        == TRUE)  {
        *r = 0; *g = 0; *b = 0;
    }

    if (
        DrawButton(
            x + 100,
            y + 20,
            50, 20, 0, 32, 0,
            text1, 16, 32, 16, taskid)
        == TRUE)  {
        *r = 221.0f / 255.0f * 16.0f;
        *g = 72.0f / 255.0f * 32.0f;
        *b = 20.0f / 255.0f * 16.0f;
    }

    return 0;
}