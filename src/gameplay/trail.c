#include "nds_types.h"
void Trail_Init(void) { g_trail_active = 0; g_trail_entity_id = 0; g_trail_type = 0; g_trailfx_count = 0; }
void Trail_Start(void) { g_trail_active = 1; g_trailfx_count = 0; }
void Trail_Update(void) {
    if (!g_trail_active) return;
    u32 i;
    for (i = 0; i < g_trailfx_count; i++) {
        if (!g_trailfxs[i].active) continue;
        g_trailfxs[i].timer++;
        if (g_trailfxs[i].timer >= 8) g_trailfxs[i].active = 0;
    }
}
void Trail_Draw(void) {
    u32 i;
    for (i = 0; i < g_trailfx_count; i++)
        if (g_trailfxs[i].active)
            OAM_AddSprite(g_trailfxs[i].x >> 8, g_trailfxs[i].y >> 8, g_trailfxs[i].tile, 0x2000);
}
void Trail_Stop(void) { g_trail_active = 0; }
s32 Trail_IsActive(u32 index) { (void)index; return g_trail_active; }
u32 Trail_GetCount(void) { return g_trailfx_count; }
u32 Trail_GetType(void) { return g_trail_type; }
u32 Trail_GetEntityId(void) { return g_trail_entity_id; }
void Trail_Clear(void) { g_trailfx_count = 0; }
void Trail_Reset(void) { g_trail_active = 0; g_trailfx_count = 0; g_trail_entity_id = 0; g_trail_type = 0; }
