/* Touch subsystem — TouchState_Producer.
 *
 * Fun: FUN_0203363c @ 0x0203363c (140 bytes)
 *   Initializes a dispatcher node structure with given parameters.
 *   Uses raw offsets because DispatcherNode layout is not fully mapped.
 */

#include "touch_state.h"

extern void FUN_020082e8(int, void*, int);

ARM9 void TouchState_Producer(void* node_raw, int param2, short param3,
                               u32 param4, u32 param5, u32 param6, u32 param7,
                               u32 param8, u32 param9)
{
    u8* node = (u8*)node_raw;
    FUN_020082e8(0, node, 0x48);
    *(u32*)(node + 0x10) = param2;
    *(u16*)(node + 0x2a) = 0x10;
    *(u32*)(node + 0x2c) = param4;
    *(short*)(node + 0x08) = param3;
    *(u32*)(node + 0x00) = param5;
    *(u32*)(node + 0x30) = param6;
    *(u32*)(node + 0x34) = param7;
    *(u32*)(node + 0x3c) = param8;
    *(u32*)(node + 0x40) = param9;
    *(u32*)(node + 0x1c) = *(u32*)(param2 + *(int*)(param2 + 4) + param3 * 8 + 4);
}
