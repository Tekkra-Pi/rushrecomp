#include "nds_types.h"
static u32 s_count;
void SpeedLines_Init(void) { s_count = 0; g_speedlines_active = 0; }
void SpeedLines_Start(void) { g_speedlines_active = 1; s_count = 0; }
void SpeedLines_Stop(void) { g_speedlines_active = 0; }
void SpeedLines_Update(void) {
    if (!g_speedlines_active) return;
    u32 i;
    for (i = 0; i < 8; i++) {
        if (s_count < 32) {
            g_speedlines[s_count].x = (g_camera_x + ((i * 73) % 256));
            g_speedlines[s_count].y = g_camera_y + ((i * 47) % 192);
            g_speedlines[s_count].vel_x = -0x400;
            g_speedlines[s_count].timer = 0; g_speedlines[s_count].active = 1;
            s_count++;
        }
    }
    for (i = 0; i < s_count; i++) {
        if (!g_speedlines[i].active) continue;
        g_speedlines[i].x += g_speedlines[i].vel_x;
        g_speedlines[i].timer++;
        if (g_speedlines[i].timer >= 8) g_speedlines[i].active = 0;
    }
}
void SpeedLines_Draw(void) {
    u32 i;
    for (i = 0; i < s_count; i++)
        if (g_speedlines[i].active)
            OAM_AddSprite(g_speedlines[i].x >> 8, g_speedlines[i].y >> 8, 0x300, 0x2000);
}
u32 SpeedLines_IsActive(u32 index) { (void)index; return g_speedlines_active; }
void SpeedLines_Reset(void) { s_count = 0; g_speedlines_active = 0; }
