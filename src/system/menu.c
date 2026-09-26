/* Menu system functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern u32 g_menu_index;
extern MenuItemEntry g_menu_items[];
extern u16 g_key_current, g_key_previous;
extern s32 g_menuscroll_offset;
extern s32 g_menuscroll_speed;

s32 Menu_Update(void) {
    u16 pressed = g_key_current & ~g_key_previous;
    s32 changed = 0;

    if (pressed & (KEY_UP | KEY_DUP)) {
        if (g_menu_index > 0) {
            g_menu_index--;
            changed = 1;
        }
    }
    if (pressed & (KEY_DOWN | KEY_DDOWN)) {
        g_menu_index++;
        changed = 1;
    }
    if (pressed & (KEY_A | KEY_START)) {
        if (g_menu_items[g_menu_index].callback) {
            g_menu_items[g_menu_index].callback();
        }
    }

    /* Auto-scroll the menu display */
    g_menuscroll_offset += g_menuscroll_speed;

    return changed;
}
