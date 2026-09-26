/* copy_32byte_aligned @ 0x020083ac (48 bytes)
 * Copies memory in 32-byte aligned blocks.
 * Args: r0=dst, r1=src, r2=len
 * Evidence: CONFIRMED-STATIC from extract.py
 * Uses ldmia/stmia for 32-byte-at-a-time copy with 4-byte remainder. */

#include "nds_types.h"

void copy_32byte_aligned(u32 dst, u32 src, u32 len) {
    u8 *s = (u8 *)src;
    u8 *end = (u8 *)(src + len);
    u32 blocks = len >> 5;

    while (blocks--) {
        u32 *d32 = (u32 *)dst;
        u32 *s32 = (u32 *)s;
        d32[0] = s32[0];
        d32[1] = s32[1];
        d32[2] = s32[2];
        d32[3] = s32[3];
        d32[4] = s32[4];
        d32[5] = s32[5];
        d32[6] = s32[6];
        d32[7] = s32[7];
        dst += 32;
        s += 32;
    }

    while (s < end) {
        *(u8 *)dst = *s;
        dst++;
        s++;
    }
}
