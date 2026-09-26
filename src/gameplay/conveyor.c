/* Gameplay level conveyor functions. */
#include "nds_types.h"

static u32 s_count;

void Conveyor_Init(void) { s_count = 0; }

s32 Conveyor_Add(s32 x, s32 y, s32 w, s32 speed) {
    if (s_count >= 16) return -1;
    ConveyorEntry *c = (ConveyorEntry*)&g_conveyor_count;
    c->x = x; c->y = y; c->w = w; c->speed = speed; c->active = 1;
    s_count++; return s_count - 1;
}

void Conveyor_Update(void) {
    u32 i;
    for (i = 0; i < s_count; i++) {
        ConveyorEntry *c = (ConveyorEntry*)&g_conveyor_count;
        if (!c->active) continue;
        s32 px, py; Player_GetPosition(&px, &py);
        s32 dx = px - c->x;
        if (dx >= 0 && dx <= c->w) {
            s32 dy = py - c->y;
            if (dy >= -0x400 && dy <= 0x400)
                Player_SetVX(Player_GetVX() + c->speed);
        }
    }
}

void Conveyor_Remove(u32 index) { (void)index; }
u32 Conveyor_GetCount(void) { return s_count; }
void Conveyor_Clear(void) { s_count = 0; }
void Conveyor_Reset(void) { s_count = 0; }
