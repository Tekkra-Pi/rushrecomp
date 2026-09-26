/* HUD/UI system functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern u32 g_hud_update_flags;
extern u32 g_hud_update_timer;
extern u32 g_hudscore_visible;
extern u32 g_hudrings_visible;
extern u32 g_hudtime_visible;
extern u32 g_hudboost_visible;
extern u32 g_hudlives_visible;

void HUD_Draw(void) {
    g_hud_update_flags = 0;
    if (g_hudscore_visible) {
        extern void HudScoreIcon_Draw(void);
        HudScoreIcon_Draw();
    }
    if (g_hudrings_visible) {
        extern void HudRingIcon_Draw(void);
        HudRingIcon_Draw();
    }
    if (g_hudtime_visible) {
        extern void HudTimerIcon_Draw(void);
        HudTimerIcon_Draw();
    }
    if (g_hudboost_visible) {
        extern void HudBoostIcon_Draw(void);
        HudBoostIcon_Draw();
    }
    if (g_hudlives_visible) {
        extern void HudLivesIcon_Draw(void);
        HudLivesIcon_Draw();
    }
}
