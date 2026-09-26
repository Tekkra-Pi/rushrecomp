#include "nds_types.h"
static u32 s_count;
void SysCallback_Init(void) { s_count = 0; }
s32 SysCallback_Add(void (*func)(void)) {
    (void)func;
    if (s_count >= 8) return -1;
    s_count++; return s_count - 1;
}
void SysCallback_Run(void) {
    u32 i;
    for (i = 0; i < s_count; i++) {
        if (g_callbacks[i].active && g_callbacks[i].func) g_callbacks[i].func();
    }
}
void SysCallback_Remove(u32 index) { if (index < s_count) g_callbacks[index].active = 0; }
u32 SysCallback_GetCount(void) { return s_count; }
void SysCallback_Clear(void) { s_count = 0; }
void SysCallback_Reset(void) { s_count = 0; }
