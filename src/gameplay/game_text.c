#include "nds_types.h"
static u32 s_font, s_color, s_scale;
void Text_Init(void) { s_font = 0; s_color = 0; s_scale = 1; }
void Text_Print(void) { }
void Text_PrintCentered(void) { }
void Text_PrintNumber(void) { }
void Text_PrintHex(void) { }
void Text_SetFont(u32 index, u32 value) { (void)index; s_font = value; }
void Text_SetColor(u32 index, u32 value) { (void)index; s_color = value; }
void Text_SetScale(u32 index, u32 value) { (void)index; s_scale = value; }
u32 Text_GetFont(void) { return s_font; }
u32 Text_GetColor(void) { return s_color; }
u32 Text_GetScale(void) { return s_scale; }
u32 Text_GetWidth(void) { return 8 * s_scale; }
u32 Text_GetHeight(void) { return 8 * s_scale; }
void Text_Clear(void) { }
void Text_Reset(void) { s_font = 0; s_color = 0; s_scale = 1; }
