/* System task functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern TaskEntry g_tasks[];

#define MAX_TASKS 16

void Task_Init(void) {
    u32 i;
    for (i = 0; i < MAX_TASKS; i++) {
        g_tasks[i].active = 0;
        g_tasks[i].func = NULL;
        g_tasks[i].priority = 0;
        g_tasks[i].paused = 0;
    }
}

s32 Task_Create(void (*func)(void)) {
    u32 i;
    for (i = 0; i < MAX_TASKS; i++) {
        if (!g_tasks[i].active) {
            g_tasks[i].active = 1;
            g_tasks[i].func = func;
            g_tasks[i].id = i;
            g_tasks[i].priority = 0;
            g_tasks[i].paused = 0;
            return (s32)i;
        }
    }
    return -1;
}

void Task_Destroy(void) {
    /* Destroy all inactive tasks */
    u32 i;
    for (i = 0; i < MAX_TASKS; i++) {
        if (!g_tasks[i].active) {
            g_tasks[i].func = NULL;
        }
    }
}

void Task_Update(void) {
    u32 i;
    for (i = 0; i < MAX_TASKS; i++) {
        if (g_tasks[i].active && !g_tasks[i].paused && g_tasks[i].func) {
            g_tasks[i].func();
        }
    }
}

void Task_Pause(void) {
    u32 i;
    for (i = 0; i < MAX_TASKS; i++) {
        g_tasks[i].paused = 1;
    }
}

void Task_Resume(void) {
    u32 i;
    for (i = 0; i < MAX_TASKS; i++) {
        g_tasks[i].paused = 0;
    }
}

u32 Task_GetCount(void) {
    u32 count = 0, i;
    for (i = 0; i < MAX_TASKS; i++) {
        if (g_tasks[i].active) count++;
    }
    return count;
}
