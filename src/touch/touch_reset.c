/* Touch subsystem — TouchState_ResetEnable.
 *
 * Fun: FUN_02033d58 @ 0x02033d58 (80 bytes)
 *   Walks controller nodes, returns first active node or 0.
 */

#include "touch_state.h"

extern u32 DAT_02033da8;
extern u32 DAT_02033dac;

ARM9 s32 TouchState_ResetEnable(void)
{
    u32* head_ptr = (u32*)(u32)&DAT_02033da8;
    u32* end_ptr  = (u32*)(u32)&DAT_02033dac;
    u32 head = *head_ptr;
    u32 end  = *end_ptr;
    u32 node = *(u32*)(head + 4);

    if (node == end) return 0;

    while (1) {
        s32 idx = *(s32*)(node + 8);
        if (idx > -1) {
            if (idx < 5) return (s32)node;
            if (idx > 6) return (s32)node;
        }
        node = *(u32*)(node + 4);
        if (node == end) return 0;
    }
}
