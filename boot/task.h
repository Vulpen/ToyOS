#ifndef TASK_H
#define TASK_H

int TasksLength = 0;

#define task_type_void 0
#define task_type_string_buffer 1

struct Task {
    // 0 to 5 with 0 being highest priority
    int priority;
    int type;
    char param1[10000];
    int param2;

    // Function pointers
    int (*function_void)(void);
    int (*function_string_buffer)(char*, int*);
};

struct Task tasks[256];

#endif