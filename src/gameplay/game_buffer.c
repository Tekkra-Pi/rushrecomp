/* Gameplay buffer functions. */
#include "nds_types.h"

void Buffer_Init(void) { g_queue_head = 0; g_queue_tail = 0; }
s32 Buffer_Create(void) { return 0; }
void Buffer_Destroy(void) { }
u32 Buffer_Write(void) { if (g_queue_tail >= 256) return 0; g_queue_tail++; return 1; }
u32 Buffer_Read(void) { if (g_queue_head >= g_queue_tail) return 0; g_queue_head++; return 1; }
void Buffer_Clear(void) { g_queue_head = 0; g_queue_tail = 0; }
u32 Buffer_GetAvailable(void) { return g_queue_tail - g_queue_head; }
