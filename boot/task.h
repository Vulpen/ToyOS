#ifndef TASK_H
#define TASK_H

#define task_params_length 10
int TasksLength = 0;


struct Task {
    // 0 to 5 with 0 being highest priority
    int priority;
    int taskId;
    char ca1[100];
    int i1;

    // Function pointers
    int (*function)(int);
    // int (*function_void)(void);
    // int (*function_string_buffer)(char*, int*);
};

struct Task tasks[256];
int iparams[100] = { 10 };  // Extra parameters for tasks

int ClearScreenTask(int taskid);
void CloseTask(int taskid);
int DrawMouseTask(int taskid);
int HandleKeyboardTask(int taskid);
int TestGraphicalElementsTask(int taskid);

#endif