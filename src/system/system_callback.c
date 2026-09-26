/* System callback functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern CallbackEntry g_callbacks[];

#define MAX_CALLBACKS 16

void Callback_Init(void) {
    u32 i;
    for (i = 0; i < MAX_CALLBACKS; i++) {
        g_callbacks[i].active = 0;
        g_callbacks[i].func = NULL;
        g_callbacks[i].type = 0;
    }
}

s32 Callback_Register(void (*func)(void)) {
    u32 i;
    for (i = 0; i < MAX_CALLBACKS; i++) {
        if (!g_callbacks[i].active) {
            g_callbacks[i].active = 1;
            g_callbacks[i].func = func;
            g_callbacks[i].type = 0;
            return (s32)i;
        }
    }
    return -1;
}

void Callback_Unregister(void) {
    u32 i;
    for (i = 0; i < MAX_CALLBACKS; i++) {
        g_callbacks[i].active = 0;
        g_callbacks[i].func = NULL;
    }
}

void Callback_Invoke(void) {
    u32 i;
    for (i = 0; i < MAX_CALLBACKS; i++) {
        if (g_callbacks[i].active && g_callbacks[i].func) {
            g_callbacks[i].func();
        }
    }
}

void Callback_InvokeAll(void) {
    Callback_Invoke();
}

u32 Callback_GetCount(void) {
    u32 count = 0, i;
    for (i = 0; i < MAX_CALLBACKS; i++) {
        if (g_callbacks[i].active) count++;
    }
    return count;
}

void Callback_Clear(void) {
    Callback_Unregister();
}
