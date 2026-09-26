#include "nds_types.h"
void MenuBg_Init(void) { g_menubg_timer = 0; }
void MenuBg_Update(void) { g_menubg_timer++; }
void MenuBg_Draw(void) { Display_FillRect(0, 0, 256, 192, 0); }
void MenuBg_SetTile(u32 tile) { (void)tile; }
void MenuBg_Reset(void) { g_menubg_timer = 0; }
