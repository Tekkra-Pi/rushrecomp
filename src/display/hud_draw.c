/* HUD draw helper functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern u32 g_oam_count;

void HUD_DrawText(void) {
    /* Draw text sprites for HUD elements */
}

void HUD_DrawNumber(void) {
    /* Draw numeric value sprites for score/rings/time */
}

void HUD_DrawIcon(void) {
    /* Draw icon sprites for HUD */
    extern void HudScoreIcon_Draw(void);
    extern void HudRingIcon_Draw(void);
    extern void HudTimerIcon_Draw(void);
    HudScoreIcon_Draw();
    HudRingIcon_Draw();
    HudTimerIcon_Draw();
}
