/* Camera trigger functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern s32 Player_GetX(void);
extern s32 Player_GetY(void);

#define CAMTRIG_MAX 16

static CameraTriggerEntry s_camtrigs[CAMTRIG_MAX];

void CameraTrigger_Init(void) { g_camtrigger_count = 0; }

s32 CameraTrigger_Add(s32 x, s32 y, u32 param) {
    if (g_camtrigger_count >= CAMTRIG_MAX) return -1;
    u32 i = g_camtrigger_count;
    s_camtrigs[i].x = x;
    s_camtrigs[i].y = y;
    s_camtrigs[i].w = 0x400;
    s_camtrigs[i].h = 0x400;
    s_camtrigs[i].mode = param;
    s_camtrigs[i].triggered = 0;
    s_camtrigs[i].active = 1;
    g_camtrigger_count++;
    return i;
}

void CameraTrigger_Remove(u32 index) {
    if (index < g_camtrigger_count) s_camtrigs[index].active = 0;
}

s32 CameraTrigger_Check(void) {
    u32 i;
    for (i = 0; i < g_camtrigger_count; i++) {
        if (!s_camtrigs[i].active || s_camtrigs[i].triggered) continue;
        s32 px = Player_GetX();
        s32 py = Player_GetY();
        if (px >= s_camtrigs[i].x && px <= s_camtrigs[i].x + s_camtrigs[i].w &&
            py >= s_camtrigs[i].y && py <= s_camtrigs[i].y + s_camtrigs[i].h) {
            s_camtrigs[i].triggered = 1;
            return i;
        }
    }
    return -1;
}

u32 CameraTrigger_GetMode(void) {
    u32 i;
    for (i = 0; i < g_camtrigger_count; i++) {
        if (s_camtrigs[i].triggered) return s_camtrigs[i].mode;
    }
    return 0;
}

s32 CameraTrigger_IsTriggered(void) {
    u32 i;
    for (i = 0; i < g_camtrigger_count; i++) {
        if (s_camtrigs[i].triggered) return 1;
    }
    return 0;
}

u32 CameraTrigger_GetCount(void) { return g_camtrigger_count; }

void CameraTrigger_Clear(void) { g_camtrigger_count = 0; }
void CameraTrigger_Reset(void) {
    u32 i;
    for (i = 0; i < g_camtrigger_count; i++) s_camtrigs[i].triggered = 0;
}
