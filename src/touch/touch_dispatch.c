/* Touch subsystem — TouchState_Dispatcher.
 *
 * Fun: FUN_02033b98 @ 0x02033b98 (436 bytes)
 *   Main touch dispatcher: validates input, allocates buffer for
 *   touch data, copies data via middleware, returns status.
 *
 * Register mapping (from disassembly):
 *   r4 = subsys pointer (base of the touch subsystem structure)
 *   sp+0 = allocated buffer pointer (local)
 *   sp+4 = DAT_02033d54 value (local)
 *
 * Subsystem struct offsets (reconstructed):
 *   +0x08  status/error code
 *   +0x0c  flags (bit0=initialized, bit1=validated)
 *   +0x10  param1 (passed to FUN_0200bb70)
 *   +0x14  param2 (passed to FUN_0200bb70)
 *   +0x18  validation buffer (0x48 bytes passed to FUN_0200be70)
 *   +0x38  start offset
 *   +0x3c  end offset
 *   +0x5c  allocated buffer pointer
 *   +0x60  computed size
 */

#include "touch_state.h"

extern void FUN_0200be70(void*);
extern int FUN_0200bb70(void*, u32, u32);
extern u32* FUN_02034a40(u32, u32, u32);
extern u32* FUN_02006abc(int);
extern void FUN_0200831c(int, void*, u32);
extern int FUN_020067c0(u32*, u32);
extern int FUN_020067a0(void*, u32);

extern u32 DAT_02033d4c;
extern u32 DAT_02033d50;
extern u32 DAT_02033d54;

ARM9 void TouchState_Dispatcher(u8* subsys)
{
    u32* alloc_ptr;
    u32* check_ptr;
    u32 data_size;
    int result;
    u32 marker;

    /* Initialize/validate the embedded buffer */
    FUN_0200be70(subsys + 0x18);

    /* Run middleware validation with subsys params */
    result = FUN_0200bb70(subsys + 0x18,
                          *(u32*)(subsys + 0x10),
                          *(u32*)(subsys + 0x14));

    if (result == 0) {
        /* Validation failed — set error and return */
        *(u32*)(subsys + 0x08) = 0xfffffffe;  /* -2 */
        return;
    }

    /* Mark validated */
    *(u32*)(subsys + 0x0c) |= 2;

    /* Compute data size from start/end offsets */
    *(u32*)(subsys + 0x60) = *(u32*)(subsys + 0x3c) - *(u32*)(subsys + 0x38);

    if (*(u32*)(subsys + 0x5c) == 0) {
        /* No buffer yet — allocate one */
        data_size = (*(u32*)(subsys + 0x60) + 0x43) & ~0x1f;
        alloc_ptr = FUN_02034a40(DAT_02033d4c, data_size, 0x20);
        check_ptr = FUN_02006abc(0);

        if (check_ptr != alloc_ptr) {
            /* Copy data into allocated buffer */
            FUN_0200831c(DAT_02033d50, alloc_ptr, data_size);

            /* Write size header and marker footer */
            marker = DAT_02033d54;
            *alloc_ptr = data_size;
            *(u32*)((u8*)alloc_ptr + (data_size - 4)) = marker;

            /* Validate the allocated buffer */
            result = FUN_020067c0(alloc_ptr, data_size);
            if (result == 0) {
                *(u32*)(subsys + 0x5c) = (u32)(alloc_ptr + 8);  /* skip 32-byte header */
            }
        }
    }

    /* Check if buffer is now available */
    if (*(u32*)(subsys + 0x5c) == 0) {
        *(u32*)(subsys + 0x08) = 0xfffffffd;  /* -3 */
        return;
    }

    /* Run final middleware pass */
    result = FUN_020067a0(subsys + 0x18, *(u32*)(subsys + 0x60));

    if (result == -1) {
        *(u32*)(subsys + 0x08) = 0xffffffff;  /* -1 */
    } else {
        *(u32*)(subsys + 0x08) = 3;  /* success */
    }
}
