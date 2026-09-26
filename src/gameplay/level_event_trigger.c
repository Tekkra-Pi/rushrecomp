/* Gameplay level level event trigger functions. */
#include "nds_types.h"


void LevelEventTrigger_Init(void) { g_levtrig_count = 0; }

s32 LevelEventTrigger_Add(s32 x, s32 y, s32 w, s32 h, u32 event_type, u32 param) {
    if (g_levtrig_count >= 16) return -1;
    u32 i = g_levtrig_count;
    g_levtrigs[i].x = x; g_levtrigs[i].y = y;
    g_levtrigs[i].w = w; g_levtrigs[i].h = h;
    g_levtrigs[i].event_type = event_type; g_levtrigs[i].param = param;
    g_levtrigs[i].triggered = 0; g_levtrigs[i].active = 1;
    g_levtrig_count++; return i;
}

void LevelEventTrigger_Update(void) {
    u32 i;
    for (i = 0; i < g_levtrig_count; i++) {
        if (!g_levtrigs[i].active || g_levtrigs[i].triggered) continue;
        s32 px, py;
        Player_GetPosition(&px, &py);
        if (px >= g_levtrigs[i].x && px <= g_levtrigs[i].x + g_levtrigs[i].w &&
            py >= g_levtrigs[i].y && py <= g_levtrigs[i].y + g_levtrigs[i].h) {
            g_levtrigs[i].triggered = 1;
            EventSys_Trigger(g_levtrigs[i].event_type, g_levtrigs[i].param);
        }
    }
}

void LevelEventTrigger_Remove(u32 i) { if (i < g_levtrig_count) g_levtrig_count--; }
u32 LevelEventTrigger_GetCount(void) { return g_levtrig_count; }
void LevelEventTrigger_Clear(void) { g_levtrig_count = 0; }
void LevelEventTrigger_Reset(void) { g_levtrig_count = 0; }
