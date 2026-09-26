/* Touch subsystem — TouchState_Init.
 *
 * Fun: FUN_02033ddc @ 0x02033ddc (96 bytes)
 *   Sets up the touch pipeline, returns computed size or -1.
 */

#include "touch_state.h"

/* External stubs */
extern void FUN_0200be70(void* dst);
extern u32 FUN_0200bb20(void* dst, u32 param);
extern u32 FUN_0200bad8(void* dst);

ARM9 int TouchState_Init(u32 param)
{
    u8 buf[0x50];
    s32* p_start = (s32*)(buf + 0x20);
    s32* p_end   = (s32*)(buf + 0x24);
    s32 result;
    s32 check;

    FUN_0200be70(buf);
    result = FUN_0200bb20(buf, param);
    if (result != 0) {
        s32 size = *p_end - *p_start;
        check = FUN_0200bad8(buf);
        if (check == 0) {
            size = -1;
        }
        return size;
    }
    return -1;
}
