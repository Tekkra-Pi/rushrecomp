/* Cam trigger functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern s32 Player_GetX(void);
extern s32 Player_GetY(void);

#define CAMTRIGGER_COUNT_MAX 16

static CameraTriggerEntry s_camtrigger_entries[CAMTRIGGER_COUNT_MAX];
static u32 s_camtrigger_count;

void CamTrigger_Init(void) { s_camtrigger_count = 0; }

s32 CamTrigger_Add(s32 x, s32 y, u32 param) {
    if (s_camtrigger_count >= CAMTRIGGER_COUNT_MAX) return -1;
    u32 i = s_camtrigger_count;
    s_camtrigger_entries[i].x = x;
    s_camtrigger_entries[i].y = y;
    s_camtrigger_entries[i].w = 0x400;
    s_camtrigger_entries[i].h = 0x400;
    s_camtrigger_entries[i].mode = param;
    s_camtrigger_entries[i].triggered = 0;
    s_camtrigger_entries[i].active = 1;
    s_camtrigger_count++;
    return i;
}

void CamTrigger_Update(void) {
    u32 i;
    for (i = 0; i < s_camtrigger_count; i++) {
        if (!s_camtrigger_entries[i].active || s_camtrigger_entries[i].triggered) continue;
        s32 px = Player_GetX();
        s32 py = Player_GetY();
        if (px >= s_camtrigger_entries[i].x && px <= s_camtrigger_entries[i].x + s_camtrigger_entries[i].w &&
            py >= s_camtrigger_entries[i].y && py <= s_camtrigger_entries[i].y + s_camtrigger_entries[i].h) {
            s_camtrigger_entries[i].triggered = 1;
        }
    }
}

void CamTrigger_Remove(u32 index) {
    if (index < s_camtrigger_count) s_camtrigger_entries[index].active = 0;
}

u32 CamTrigger_GetCount(void) { return s_camtrigger_count; }

void CamTrigger_Clear(void) { s_camtrigger_count = 0; }
void CamTrigger_Reset(void) {
    u32 i;
    for (i = 0; i < s_camtrigger_count; i++) s_camtrigger_entries[i].triggered = 0;
}
