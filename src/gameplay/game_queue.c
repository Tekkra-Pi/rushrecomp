/* Gameplay queue functions. */
#include "nds_types.h"

void Queue_Init(void) { g_queue_head = 0; g_queue_tail = 0; }
s32 Queue_Enqueue(void) { if (g_queue_tail >= 256) return -1; g_queue_tail++; return 0; }
u32 Queue_Dequeue(void) { if (g_queue_head >= g_queue_tail) return 0; g_queue_head++; return 1; }
u32 Queue_Peek(void) { return g_queue_head >= g_queue_tail ? 0 : 1; }
s32 Queue_IsEmpty(void) { return g_queue_head >= g_queue_tail ? 1 : 0; }
s32 Queue_IsFull(void) { return g_queue_tail >= 256 ? 1 : 0; }
u32 Queue_GetCount(void) { return g_queue_tail - g_queue_head; }
void Queue_Clear(void) { g_queue_head = 0; g_queue_tail = 0; }
