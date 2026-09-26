/* Gameplay level event trigger functions. */
#include "nds_types.h"

extern s32 Player_GetX(void);
extern s32 Player_GetY(void);

void EventTrigger_Init(void) { }

s32 EventTrigger_Add(s32 x, s32 y, u32 param) {
    u32 i = g_actiontrig_count;
    g_evttrigs[i].x = x;
    g_evttrigs[i].y = y;
    g_evttrigs[i].w = 0x400;
    g_evttrigs[i].h = 0x400;
    g_evttrigs[i].event_type = param;
    g_evttrigs[i].param = 0;
    g_evttrigs[i].triggered = 0;
    g_evttrigs[i].active = 1;
    g_actiontrig_count++;
    return i;
}

void EventTrigger_Update(void) {
    u32 i;
    for (i = 0; i < g_actiontrig_count; i++) {
        if (!g_evttrigs[i].active || g_evttrigs[i].triggered) continue;
        s32 px = Player_GetX();
        s32 py = Player_GetY();
        if (px >= g_evttrigs[i].x && px <= g_evttrigs[i].x + g_evttrigs[i].w &&
            py >= g_evttrigs[i].y && py <= g_evttrigs[i].y + g_evttrigs[i].h) {
            g_evttrigs[i].triggered = 1;
        }
    }
}

void EventTrigger_Remove(u32 index) {
    if (index < g_actiontrig_count) g_evttrigs[index].active = 0;
}

u32 EventTrigger_GetCount(void) { return g_actiontrig_count; }

void EventTrigger_Clear(void) { g_actiontrig_count = 0; }
void EventTrigger_Reset(void) {
    u32 i;
    for (i = 0; i < g_actiontrig_count; i++) g_evttrigs[i].triggered = 0;
}
