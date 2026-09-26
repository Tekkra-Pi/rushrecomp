/* Save data functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern u32 g_savemgr_count;
extern u32 g_savemgr_dirty;
extern char g_savemgr_keys[64][32];
extern u32 g_savemgr_values[64];

u32 Save_CalculateChecksum(void);

void Save_Init(void) {
    u32 i;
    g_savemgr_count = 0;
    g_savemgr_dirty = 0;
    for (i = 0; i < 64; i++) {
        g_savemgr_keys[i][0] = '\0';
        g_savemgr_values[i] = 0;
    }
}

s32 Save_Load(void) {
    /* Load save data from SRAM/flash */
    extern void SRAM_Read(u32 offset, void *dst, u32 len);
    u32 buf[4];
    SRAM_Read(0, buf, sizeof(buf));
    if (buf[0] != 0x53564153) return -1;
    g_savemgr_count = buf[1];
    if (g_savemgr_count > 64) g_savemgr_count = 64;
    SRAM_Read(16, g_savemgr_values, g_savemgr_count * 4);
    return 0;
}

void Save_Write(void) {
    extern void SRAM_Write(u32 offset, const void *src, u32 len);
    u32 hdr[4];
    hdr[0] = 0x53564153;
    hdr[1] = g_savemgr_count;
    hdr[2] = Save_CalculateChecksum();
    hdr[3] = 0;
    SRAM_Write(0, hdr, 16);
    SRAM_Write(16, g_savemgr_values, g_savemgr_count * 4);
    g_savemgr_dirty = 0;
}

u32 Save_CalculateChecksum(void) {
    u32 sum = 0;
    u32 i;
    u32 *vals = (u32*)g_savemgr_values;
    for (i = 0; i < g_savemgr_count; i++) {
        sum ^= vals[i];
        sum += i * 0x9E3779B9;
    }
    return sum;
}
