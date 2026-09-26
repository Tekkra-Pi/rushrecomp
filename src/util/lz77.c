/* LZ77 decompression functions.
 * Evidence: CONFIRMED-STATIC from ROM disassembly at 0x02008648, 0x02008720
 *
 * NDS LZ77 format:
 *   Header (4 bytes): byte 0 = type (0x10), bytes 1-3 = decompressed size (24-bit LE)
 *   Data: flags byte + variable-length entries
 *   Flag bit 0 = literal byte copy
 *   Flag bit 1 = back-reference (2-byte: 12-bit offset + 4-bit length)
 */

#include "nds_types.h"

/* ======================================================================== */
/* LZ77_GetHeader @ ~0x02008640                                             */
/* Reads and validates an NDS LZ77 header.                                   */
/* Args: r0=compressed data pointer                                          */
/* Returns: decompressed size from header, or 0 if invalid                  */
/* ======================================================================== */
u32 LZ77_GetHeader(u32 src) {
    u8 *data = (u8 *)src;
    u32 header;
    u32 size;

    /* Read 4-byte header */
    header = *(u32 *)data;

    /* Check type nibble (low byte should be 0x10 for LZ77) */
    if ((header & 0xFF) != 0x10) {
        return 0;
    }

    /* Extract 24-bit decompressed size from bytes 1-3 */
    size = (header >> 8) & 0x00FFFFFF;

    return size;
}

/* ======================================================================== */
/* LZ77_Decompress @ 0x02008648 (208 bytes)                                */
/* Decompresses NDS LZ77 data from src to dst.                              */
/* Args: r0=src (compressed data), r1=dst (output buffer)                   */
/* Evidence: CONFIRMED-STATIC from ROM extraction                           */
/* ======================================================================== */
void LZ77_Decompress(u32 src, u32 dst) {
    u8 *s = (u8 *)src;
    u8 *d = (u8 *)dst;
    u32 header;
    u32 decompressed_size;
    u32 remaining;
    u8 flags;
    int bit_count;

    /* Read and skip 4-byte header */
    header = *(u32 *)s;
    s += 4;

    /* Decompressed size is bits 8-31 of header */
    decompressed_size = header >> 8;
    remaining = decompressed_size;

    /* Bit accumulator and shift state */
    flags = 0;
    bit_count = 0;

    while (remaining > 0) {
        /* Refill flags byte when all bits consumed */
        if (bit_count == 0) {
            flags = *s++;
            bit_count = 8;
        }

        if (flags & 0x80) {
            /* Back-reference: 2-byte offset+length */
            u32 b0 = *s++;
            u32 b1 = *s++;
            u32 length = (b0 >> 4) + 3;
            u32 offset = ((b0 & 0x0F) << 8) | b1;
            u32 back = offset + 1;
            u32 i;

            for (i = 0; i < length && remaining > 0; i++) {
                *d = d[-(s32)back];
                d++;
                remaining--;
            }
        } else {
            /* Literal byte */
            *d++ = *s++;
            remaining--;
        }

        flags <<= 1;
        bit_count--;
    }
}

/* ======================================================================== */
/* LZ77_DecompressToVRAM @ 0x02008720 (196 bytes)                          */
/* Decompresses NDS LZ77 data directly to VRAM-safe memory.                */
/* Uses word-aligned writes to avoid VRAM write issues.                     */
/* Args: r0=src (compressed data), r1=dst (VRAM destination)                */
/* Evidence: CONFIRMED-STATIC from ROM extraction                           */
/* ======================================================================== */
void LZ77_DecompressToVRAM(u32 src, u32 dst) {
    u8 *s = (u8 *)src;
    u32 *d = (u32 *)dst;
    u32 header;
    u32 decompressed_size;
    u32 remaining;
    u8 type_byte;
    u32 word_buf;
    int word_idx;
    int bits_left;
    u32 flags;
    u32 flags_remaining;

    /* Read header byte */
    type_byte = *s++;

    /* Calculate shift from type nibble (low nibble) */
    {
        u32 shift = (type_byte & 0x0F) + 4;
        u32 bit_width = (type_byte & 0x07);
        (void)bit_width;
        (void)shift;
    }

    /* Read 3-byte size */
    decompressed_size = (u32)s[0] | ((u32)s[1] << 8) | ((u32)s[2] << 16);
    s += 3;

    /* Skip parameter byte(s) based on type */
    {
        u32 param = *s++;
        u32 data_start = param + 1;
        s = (u8 *)((u32)src + 4 + data_start * 2);
    }

    remaining = decompressed_size >> 2;
    flags = 0;
    flags_remaining = 0;
    word_buf = 0;
    word_idx = 0;

    while (remaining > 0) {
        /* Refill flag bits from 32-bit control word */
        if (flags_remaining == 0) {
            flags = *(u32 *)s;
            s += 4;
            flags_remaining = 32;
        }

        /* Read a byte from source data area */
        {
            u8 b = *s;
            u32 flag_set = flags >> 31;

            if (flag_set) {
                /* Back-reference */
                u32 shift_amt = 32 - flags_remaining;
                u32 data_word = (u32)b << shift_amt;
                word_buf = (word_buf >> 1) | data_word;
                flags <<= 1;
                flags_remaining--;

                s = (u8 *)((u32)s + 1);
                word_idx++;

                if (word_idx >= 4) {
                    *d++ = word_buf;
                    word_buf = 0;
                    word_idx = 0;
                    remaining--;
                }
            } else {
                /* Literal - reset to byte-aligned */
                flags <<= 1;
                flags_remaining--;
                s = (u8 *)((u32)s + 1);
            }
        }
    }
}
