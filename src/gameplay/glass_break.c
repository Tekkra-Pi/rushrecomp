/* Gameplay level glass break functions. */
#include "nds_types.h"

extern void ExplosionFX_Spawn(s32 x, s32 y);

#define GLASSBREAK_MAX 16

void GlassBreak_Init(void) { g_glassbreak_count = 0; }

s32 GlassBreak_Add(s32 x, s32 y) {
    if (g_glassbreak_count >= GLASSBREAK_MAX) return -1;
    u32 i = g_glassbreak_count;
    g_glassbreaks[i].x = x;
    g_glassbreaks[i].y = y;
    g_glassbreaks[i].active = 1;
    g_glassbreak_count++;
    return i;
}

void GlassBreak_Hit(u32 index) {
    if (index >= GLASSBREAK_MAX) return;
    if (g_glassbreaks[index].active) {
        g_glassbreaks[index].active = 0;
        ExplosionFX_Spawn(g_glassbreaks[index].x, g_glassbreaks[index].y);
    }
}

void GlassBreak_Update(void) { }

void GlassBreak_Remove(u32 index) {
    if (index < GLASSBREAK_MAX) g_glassbreaks[index].active = 0;
}

u32 GlassBreak_GetCount(void) { return g_glassbreak_count; }

void GlassBreak_Clear(void) { g_glassbreak_count = 0; }
void GlassBreak_Reset(void) { g_glassbreak_count = 0; }
