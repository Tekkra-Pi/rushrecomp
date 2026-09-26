

#include "nds_types.h"

/* Memory copy helpers used by touch subsystem.
 * Evidence: INFERRED from touch_main.c usage and disassembly */

/* copy_32byte_aligned @ 0x020083ac
 * Copies len bytes, optimized for 32-byte aligned blocks.
 * Args: r0=dst, r1=src, r2=len
 * Used when len is multiple of 32 (n32 in touch_main.c). */
void copy_32byte_aligned(u32 *dst, u32 *src, u32 len) {
    u32 *end = (u32 *)((u8 *)src + len);
    u32 *block_end = (u32 *)((u8 *)src + (len & ~0x1f));
    
    /* Copy 32-byte blocks */
    while (src < block_end) {
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
        dst[4] = src[4];
        dst[5] = src[5];
        dst[6] = src[6];
        dst[7] = src[7];
        dst += 8;
        src += 8;
    }
    
    /* Copy remaining 4-byte words */
    while (src < end) {
        *dst++ = *src++;
    }
}

/* copy_4byte_aligned @ 0x02008330
 * Copies len bytes, optimized for 4-byte aligned blocks.
 * Args: r0=dst, r1=src, r2=len
 * Used when len is multiple of 4 but not 32 (n4 in touch_main.c). */
void copy_4byte_aligned(u32 *dst, u32 *src, u32 len) {
    u32 *end = (u32 *)((u8 *)src + len);
    
    while (src < end) {
        *dst++ = *src++;
    }
}

/* copy_2byte_aligned @ 0x02008300
 * Copies len bytes, optimized for 2-byte aligned blocks.
 * Args: r0=dst, r1=src, r2=len
 * Used when len is multiple of 2 but not 4 (n2 in touch_main.c). */
void copy_2byte_aligned(u16 *dst, u16 *src, u32 len) {
    u32 i;
    
    for (i = 0; i < len; i += 2) {
        dst[i / 2] = src[i / 2];
    }
}

/* copy_1byte_remainder @ 0x02008500
 * Copies remaining bytes one at a time with endianness handling.
 * Args: r0=dst, r1=src, r2=len
 * Used for the final 0-3 bytes (len & 3 in touch_main.c). */
void copy_1byte_remainder(u8 *dst, u8 *src, u32 len) {
    u32 i;
    
    if (len == 0) return;
    
    /* Handle unaligned copy with byte swapping */
    for (i = 0; i < len; i++) {
        dst[i] = src[i];
    }
}
