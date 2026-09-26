#include "nds_types.h"
void ShieldBubble_Init(void) { g_shieldbubble_active = 0; g_shieldbubble_alpha = 0; g_shieldbubble_timer = 0; }
void ShieldBubble_Start(void) { g_shieldbubble_active = 1; g_shieldbubble_alpha = 0x100; g_shieldbubble_timer = 300; }
void ShieldBubble_Update(void) {
    if (!g_shieldbubble_active) return;
    g_shieldbubble_timer++;
    if (g_shieldbubble_timer > 240) g_shieldbubble_alpha -= 4;
    if (g_shieldbubble_timer >= 300) { g_shieldbubble_active = 0; g_shieldbubble_alpha = 0; }
}
void ShieldBubble_Draw(void) {
    if (!g_shieldbubble_active) return;
    s32 px, py; Player_GetPosition(&px, &py);
    OAM_AddSprite(px >> 8 - 16, py >> 8 - 16, 0x404, 0x3000);
}
void ShieldBubble_Stop(void) { g_shieldbubble_active = 0; }
s32 ShieldBubble_IsActive(u32 index) { (void)index; return g_shieldbubble_active; }
u32 ShieldBubble_GetTimer(void) { return g_shieldbubble_timer; }
s32 ShieldBubble_GetAlpha(void) { return g_shieldbubble_alpha; }
s32 ShieldBubble_CheckHit(void) {
    if (!g_shieldbubble_active) return 0;
    g_shieldbubble_active = 0; return 1;
}
void ShieldBubble_Reset(void) { g_shieldbubble_active = 0; g_shieldbubble_alpha = 0; g_shieldbubble_timer = 0; }
