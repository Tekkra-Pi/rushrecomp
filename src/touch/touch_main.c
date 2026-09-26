/* Touch subsystem — main updater (bulk memory copy).
 *
 * Fun: FUN_02033a68 @ 0x02033a68 (188 bytes)
 *   TouchState_MainUpdater: copies data in aligned chunks.
 *
 * This is an optimized memory copy that handles alignment:
 *   1. 32-byte aligned chunks (both src/dst 4-byte aligned)
 *   2. 4-byte aligned chunks
 *   3. 2-byte aligned chunks
 *   4. 1-byte remainder
 *
 * The function takes (dst, src, len) and performs a segmented copy.
 */

#include "touch_state.h"

/* External stubs for aligned memory operations */
extern void FUN_020083ac(u32 dst, u32 src, u32 len);
extern void FUN_02008330(u32 dst, u32 src, u32 len);
extern void FUN_02008300(u32 dst, u32 src, u32 len);
extern void FUN_02008500(u32 dst, u32 src, u32 len);

/* FUN_02033a68 — TouchState_MainUpdater (188 bytes)
 * Params: r0=dst, r1=src, r2=len
 *
 * Ghidra decompilation (cleaned):
 *   if (((dst | src) & 3) == 0) {
 *       // 32-byte chunks
 *       u32 n32 = len & ~0x1f;
 *       if (n32) { FUN_020083ac(dst, src, n32); dst += n32; src += n32; len &= 0x1f; }
 *       // 4-byte chunks
 *       u32 n4 = len & ~3;
 *       if (n4) { FUN_02008330(dst, src, n4); dst += n4; src += n4; len &= 3; }
 *   }
 *   // 2-byte chunks
 *   if (((dst | src) & 1) == 0) {
 *       u32 n2 = len & ~1;
 *       if (n2) { FUN_02008300(dst, src, n2); dst += n2; src += n2; len &= 1; }
 *   }
 *   // 1-byte remainder
 *   if (len) { FUN_02008500(dst, src, len); }
 */
ARM9 void TouchState_MainUpdater(u32 dst, u32 src, u32 len)
{
    u32 n;

    if (((dst | src) & 3) == 0) {
        /* 32-byte aligned copy */
        n = len & ~0x1f;
        if (n != 0) {
            FUN_020083ac(dst, src, n);
            dst += n;
            src += n;
            len &= 0x1f;
        }
        /* 4-byte aligned copy */
        n = len & ~3;
        if (n != 0) {
            FUN_02008330(dst, src, n);
            dst += n;
            src += n;
            len &= 3;
        }
    }

    /* 2-byte aligned copy */
    if (((dst | src) & 1) == 0) {
        n = len & ~1;
        if (n != 0) {
            FUN_02008300(dst, src, n);
            dst += n;
            src += n;
            len &= 1;
        }
    }

    /* 1-byte remainder */
    if (len != 0) {
        FUN_02008500(dst, src, len);
    }
}
