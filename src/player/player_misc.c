/* Player misc functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"
#include "player.h"

/* PlayerMisc_Init @ 0x02017c00 (168 bytes)
 * Initializes player misc system.
 * Args: r0=player */
void PlayerMisc_Init(void *player) {
    u32 *data = (u32*)player;
    data[46] = 0;
    data[47] = 0;
}

/* PlayerMisc_Update @ 0x02017ca8 (232 bytes)
 * Updates player misc.
 * Args: r0=player */
void PlayerMisc_Update(void *player) {
    u32 *data = (u32*)player;
    data[47]++;
}

/* PlayerMisc_GetTimer @ 0x02017d90 (296 bytes)
 * Returns misc timer.
 * Args: r0=player
 * Returns: timer */
u32 PlayerMisc_GetTimer(void *player) {
    return ((u32*)player)[47];
}

/* PlayerMisc_SetTimer @ 0x02017eb8
 * Sets misc timer.
 * Args: r0=player, r1=timer */
void PlayerMisc_SetTimer(void *player, u32 timer) {
    ((u32*)player)[47] = timer;
}

/* PlayerMisc_GetFlags @ 0x02017ef0
 * Returns misc flags.
 * Args: r0=player
 * Returns: flags */
u32 PlayerMisc_GetFlags(void *player) {
    return ((u32*)player)[46];
}

/* PlayerMisc_SetFlags @ 0x02017f20
 * Sets misc flags.
 * Args: r0=player, r1=flags */
void PlayerMisc_SetFlags(void *player, u32 flags) {
    ((u32*)player)[46] = flags;
}

/* PlayerMisc_CheckFlag @ 0x02017f60
 * Checks misc flag.
 * Args: r0=player, r1=flag
 * Returns: 1 if set, 0 otherwise */
s32 PlayerMisc_CheckFlag(void *player, u32 flag) {
    return (((u32*)player)[46] & flag) ? 1 : 0;
}

/* PlayerMisc_Reset @ 0x02017fa0
 * Resets player misc.
 * Args: r0=player */
void PlayerMisc_Reset(void *player) {
    u32 *data = (u32*)player;
    data[46] = 0;
    data[47] = 0;
}
