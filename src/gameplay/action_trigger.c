/* Gameplay level action trigger functions. */

#include "nds_types.h"

extern s32 Player_GetX(void);
extern s32 Player_GetY(void);

#define ACTIONTRIG_MAX 16

static ActionTriggerEntry s_actiontrigs[ACTIONTRIG_MAX];
static u32 s_actiontrig_count;

void ActionTrigger_Init(void) { s_actiontrig_count = 0; }

s32 ActionTrigger_Add(s32 x, s32 y, u32 param) {
    if (s_actiontrig_count >= ACTIONTRIG_MAX) return -1;
    u32 i = s_actiontrig_count;
    s_actiontrigs[i].x = x;
    s_actiontrigs[i].y = y;
    s_actiontrigs[i].w = 0x400;
    s_actiontrigs[i].h = 0x400;
    s_actiontrigs[i].event_type = param;
    s_actiontrigs[i].param = 0;
    s_actiontrigs[i].triggered = 0;
    s_actiontrigs[i].active = 1;
    s_actiontrig_count++;
    return i;
}

void ActionTrigger_Update(void) {
    u32 i;
    for (i = 0; i < s_actiontrig_count; i++) {
        if (!s_actiontrigs[i].active || s_actiontrigs[i].triggered) continue;
        s32 px = Player_GetX();
        s32 py = Player_GetY();
        if (px >= s_actiontrigs[i].x && px <= s_actiontrigs[i].x + s_actiontrigs[i].w &&
            py >= s_actiontrigs[i].y && py <= s_actiontrigs[i].y + s_actiontrigs[i].h) {
            s_actiontrigs[i].triggered = 1;
        }
    }
}

void ActionTrigger_Remove(u32 index) {
    if (index < s_actiontrig_count) s_actiontrigs[index].active = 0;
}

u32 ActionTrigger_GetCount(void) { return s_actiontrig_count; }

void ActionTrigger_Clear(void) { s_actiontrig_count = 0; }
void ActionTrigger_Reset(void) {
    u32 i;
    for (i = 0; i < s_actiontrig_count; i++) s_actiontrigs[i].triggered = 0;
}
