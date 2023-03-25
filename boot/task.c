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

void ProcessTasks() {
    int priority = 0;

    while (priority <= 5)
    {
        for(int i = 0; i < TasksLength; i++) {
            if (tasks[i].priority == priority){
                if (tasks[i].type == task_type_void) {
                    tasks[i].function_void();
                } else if (tasks[i].type == task_type_string_buffer) {
                    tasks[i].function_string_buffer(tasks[i].param1, tasks[i].param2);
                }
            }
        }

        priority++;
    }
    
}

void ClearScreenTask() {
    ClearScreen(119.0f / 255.0f * 16.0f, 41.0f / 255.0f * 32.0f, 83.0f / 255.0f * 16.0f);
}

// void DrawMouseTask