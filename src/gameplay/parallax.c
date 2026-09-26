#include "nds_types.h"
void Parallax_Init(void) { g_parallax_y = 0; }
void Parallax_Set(u32 index, u32 value) { (void)index; g_parallax_y = (s32)value; }
void Parallax_Update(void) {
    s32 cx, cy; DisplayScroll_GetPosition(&cx, &cy); g_parallax_y = cy / 2;
}
void Parallax_Reset(void) { g_parallax_y = 0; }
