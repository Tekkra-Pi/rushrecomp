#include "nds_types.h"
void Shield_Init(void) { g_shield_type = 0; }
void Shield_Activate(void) { g_shield_type = 1; }
void Shield_Update(void) { }
void Shield_Deactivate(void) { g_shield_type = 0; }
s32 Shield_IsActive(u32 index) { (void)index; return g_shield_type != 0 ? 1 : 0; }
u32 Shield_GetType(void) { return g_shield_type; }
u32 Shield_GetTimer(void) { return 0; }
s32 Shield_Use(void) {
    if (g_shield_type == 0) return 0;
    g_shield_type = 0; return 1;
}
void Shield_Draw(void) { }
u32 Shield_GetProgress(void) { return 0; }
void Shield_Reset(void) { g_shield_type = 0; }
