#include "nds_types.h"
static u32 s_vis;
void StageRender_Init(void) { s_vis = 1; }
void StageRender_Update(void) { }
void StageRender_Draw(void) { }
void StageRender_SetActive(u32 index, u32 value) { (void)index; s_vis = value; }
s32 StageRender_IsActive(u32 index) { (void)index; return s_vis; }
void StageRender_Reset(void) { s_vis = 1; }
