/* Core memory functions used throughout the engine.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

/* memset_halfword @ 0x020082e8 (24 bytes)
 * Fills memory with a 16-bit value.
 * Args: r0=value (halfword), r1=dst, r2=len (bytes)
 * Used by entity system to clear data blocks. */
void memset_halfword(u16 value, u16 *dst, u32 len) {
    u32 i;
    for (i = 0; i < len; i += 2) {
        dst[i / 2] = value;
    }
}

/* memset_words @ 0x0200831c (64 bytes)
 * Fills memory with a 32-bit value.
 * Args: r0=value, r1=dst, r2=len (bytes, must be multiple of 4) */
void memset_words(u32 value, u32 *dst, u32 len) {
    u32 *end = (u32 *)((u8 *)dst + len);
    while (dst < end) {
        *dst++ = value;
    }
}

/* memcpy_words @ 0x0200831c+16 (48 bytes)
 * Copies memory in 32-bit words.
 * Args: r0=src, r1=dst, r2=len (bytes) */
void memcpy_words(u32 *src, u32 *dst, u32 len) {
    u32 *end = (u32 *)((u8 *)src + len);
    while (src < end) {
        *dst++ = *src++;
    }
}

/* memory_block_copy @ 0x02008360 (64 bytes)
 * Optimized memory copy with 32-byte block writes.
 * Args: r0=value (fill), r1=dst, r2=len
 * First copies 32-byte blocks, then remaining words. */
void memory_block_copy(u32 value, u32 *dst, u32 len) {
    u32 *end = (u32 *)((u8 *)dst + len);
    u32 *block_end = (u32 *)((u8 *)dst + (len & ~0x1f));
    
    /* Fill 32-byte blocks (8 words at a time) */
    while (dst < block_end) {
        dst[0] = value;
        dst[1] = value;
        dst[2] = value;
        dst[3] = value;
        dst[4] = value;
        dst[5] = value;
        dst[6] = value;
        dst[7] = value;
        dst += 8;
    }
    
    /* Fill remaining words */
    while (dst < end) {
        *dst++ = value;
    }
}

/* memset_aligned @ 0x02008300
 * Simple aligned memset (already defined in copy_helpers.c)
 * This is an alias for compatibility. */
void memset_aligned(u16 value, u16 *dst, u32 len) {
    u32 i;
    for (i = 0; i < len; i += 2) {
        dst[i / 2] = value;
    }
}
