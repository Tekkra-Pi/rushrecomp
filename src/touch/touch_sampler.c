/* Touch subsystem — Sampler_ConsumeSharedSamples.
 *
 * Fun: FUN_02009900 @ 0x02009900 (440 bytes)
 *   Parses a single touch sample from either the shared-sample array
 *   (contact mode) or the per-index table (no-contact mode).
 *
 *   output[0] = flags: bit0=valid, bit1=contact
 *   output[1] = X coordinate (12-bit, shifted and pressure-scaled)
 *   output[2] = pressure/angle byte (bits 16-22 of raw sample)
 *   output[3] = Y value (no-contact path only)
 *
 * Register mapping (from disassembly):
 *   r0 = input_subsys (working)
 *   r1 = sample_idx
 *   r2 = output pointer
 *   r3, ip, lr = temps
 */

#include "touch_state.h"

ARM9 int Sampler_ConsumeSharedSamples(int input_subsys, int sample_idx, u32* output)
{
    u32 raw_sample;
    u32 flags;
    u16 x_val;
    u32 pressure_level;
    u8* alt_ptr;
    u16 alt_x;
    u32 alt_hw;

    /* Range check: 0 <= sample_idx <= 15 */
    if (sample_idx < 0 || sample_idx > 15)
        return 0;

    /* Check if sample is valid in contact bitmask */
    flags = *(u32*)(input_subsys + 0x11c4);  /* contact_mask */
    output[0] = (output[0] & ~2) | (((flags >> sample_idx) & 1) << 1);

    /* Check contact bit (bit1 of output[0]) */
    if ((output[0] & 2) == 0)
        goto no_contact;

    /* ── Contact mode: parse shared sample word ── */
    raw_sample = *(u32*)(input_subsys + 0x1180 + sample_idx * 4);

    /* bit31 = valid flag */
    output[0] = (output[0] & ~1) | ((raw_sample >> 31) & 1);

    /* bits 0-6 = X coordinate, stored at output+4 as halfword */
    x_val = (u16)(raw_sample & 0x7f);
    *(u16*)(output + 1) = x_val;
    x_val = *(u16*)(output + 1);
    x_val <<= 4;
    *(u16*)(output + 1) = x_val;

    /* bits 8-9 = pressure scaling level */
    pressure_level = (raw_sample & 0x300) >> 8;
    switch (pressure_level) {
    case 1:
        *(u16*)(output + 1) = *(u16*)(output + 1) >> 1;
        break;
    case 2:
        *(u16*)(output + 1) = *(u16*)(output + 1) >> 2;
        break;
    case 3:
        *(u16*)(output + 1) = *(u16*)(output + 1) >> 4;
        break;
    }

    /* bits 16-22 = pressure/angle byte */
    *(u8*)(output + 2) = (u8)((raw_sample & 0x7f0000) >> 16);
    return 1;

no_contact:
    /* ── No-contact mode: parse per-index table record ── */
    alt_ptr = (u8*)(input_subsys + sample_idx * 84);  /* 0x54 stride */

    /* offset 3 bit0 = valid flag */
    output[0] = (output[0] & ~1) | (alt_ptr[3] & 1);

    /* offset 2 = Y value */
    *(u8*)(output + 2) = alt_ptr[2];

    /* offset 0x24 = X + pressure halfword */
    alt_hw = *(u16*)(alt_ptr + 0x24);
    alt_x = (u16)(alt_hw & 0xff);
    *(u16*)(output + 1) = alt_x;
    alt_x = *(u16*)(output + 1);
    alt_x <<= 4;
    *(u16*)(output + 1) = alt_x;

    /* high byte of halfword = pressure scaling */
    pressure_level = (alt_hw >> 8) & 0xff;
    switch (pressure_level) {
    case 1:
        *(u16*)(output + 1) = *(u16*)(output + 1) >> 1;
        break;
    case 2:
        *(u16*)(output + 1) = *(u16*)(output + 1) >> 2;
        break;
    case 3:
        *(u16*)(output + 1) = *(u16*)(output + 1) >> 4;
        break;
    }

    /* offset 0x23 = pressure byte */
    *(u8*)(output + 2) = alt_ptr[0x23];
    return 1;
}
