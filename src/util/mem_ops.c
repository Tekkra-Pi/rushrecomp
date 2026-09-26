/* Memory utility functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

/* memset_halfword @ 0x020082e8 (24 bytes)
 * Fills memory with halfword value.
 * Args: r0=dest, r1=value, r2=count */
void memset_halfword(void *dest, u16 value, u32 count) {
    u16 *ptr = (u16*)dest;
    u32 i;
    
    for (i = 0; i < count; i++) {
        ptr[i] = value;
    }
}

/* memcpy_words @ 0x0200831c (68 bytes)
 * Copies memory in word chunks.
 * Args: r0=dest, r1=src, r2=count */
void memcpy_words(void *dest, const void *src, u32 count) {
    u32 *d = (u32*)dest;
    const u32 *s = (const u32*)src;
    u32 i;
    
    for (i = 0; i < count; i++) {
        d[i] = s[i];
    }
}

/* memory_block_copy @ 0x02008360 (416 bytes)
 * Copies a block of memory with alignment handling.
 * Args: r0=dest, r1=src, r2=size */
void memory_block_copy(void *dest, const void *src, u32 size) {
    u8 *d = (u8*)dest;
    const u8 *s = (const u8*)src;
    u32 i;
    
    /* Copy byte by byte */
    for (i = 0; i < size; i++) {
        d[i] = s[i];
    }
}

/* memset_aligned @ 0x02008500 (48 bytes)
 * Fills aligned memory with value.
 * Args: r0=dest, r1=value, r2=count */
void memset_aligned(void *dest, u32 value, u32 count) {
    u32 *ptr = (u32*)dest;
    u32 i;
    
    /* Ensure destination is aligned */
    if ((u32)dest & 3) {
        return;  /* Not aligned */
    }
    
    for (i = 0; i < count; i++) {
        ptr[i] = value;
    }
}
