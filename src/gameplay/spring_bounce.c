#include "nds_types.h"
void SpringBounce_Init(void) { g_springbounce_active = 0; g_springbounce_timer = 0; }
void SpringBounce_Start(void) { g_springbounce_active = 1; g_springbounce_timer = 0; s32 px, py; Player_GetPosition(&px, &py); g_springbounce_x = px; g_springbounce_y = py; }
void SpringBounce_Update(void) {
    if (!g_springbounce_active) return;
    g_springbounce_timer++;
    g_springbounce_y -= 0x200;
    if (g_springbounce_timer >= 30) g_springbounce_active = 0;
}
void SpringBounce_Draw(void) {
    if (!g_springbounce_active) return;
    OAM_AddSprite(g_springbounce_x >> 8 - 16, g_springbounce_y >> 8 - 16, 0x400, 0x3000);
}
void SpringBounce_Stop(void) { g_springbounce_active = 0; }
s32 SpringBounce_IsActive(u32 index) { (void)index; return g_springbounce_active; }
u32 SpringBounce_GetTimer(void) { return g_springbounce_timer; }
void SpringBounce_GetPosition(void) { }
s32 SpringBounce_GetRadius(void) { return 0x200; }
void SpringBounce_Reset(void) { g_springbounce_active = 0; g_springbounce_timer = 0; }
