/* Gameplay level save manager functions. */
#include "nds_types.h"

void SaveMgr_Init(void) { g_savemgr_dirty = 0; }

void SaveMgr_Write(const char *key, u32 value) {
    u32 i;
    for (i = 0; i < g_savemgr_count; i++) {
        if (strcmp(g_savemgr_keys[i], key) == 0) {
            g_savemgr_values[i] = value;
            g_savemgr_dirty = 1;
            return;
        }
    }
    if (g_savemgr_count < 64) {
        strncpy(g_savemgr_keys[g_savemgr_count], key, 31);
        g_savemgr_keys[g_savemgr_count][31] = '\0';
        g_savemgr_values[g_savemgr_count] = value;
        g_savemgr_count++;
        g_savemgr_dirty = 1;
    }
}

u32 SaveMgr_Read(const char *key) {
    u32 i;
    for (i = 0; i < g_savemgr_count; i++) {
        if (strcmp(g_savemgr_keys[i], key) == 0) {
            return g_savemgr_values[i];
        }
    }
    return 0;
}

void SaveMgr_Flush(void) {
    if (!g_savemgr_dirty) return;
    g_savemgr_dirty = 0;
}

s32 SaveMgr_IsDirty(void) { return g_savemgr_dirty; }
void SaveMgr_Clear(void) { g_savemgr_count = 0; g_savemgr_dirty = 0; }
void SaveMgr_Reset(void) { SaveMgr_Init(); }
