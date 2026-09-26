/* Player animation functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"
#include "player.h"

/* PlayerAnim_Init @ 0x02010000 (148 bytes)
 * Initializes player animation system.
 * Args: r0=player */
void PlayerAnim_Init(void *player) {
    u32 *data = (u32*)player;
    
    /* Clear animation state */
    data[20] = 0;  /* +0x50: current anim */
    data[21] = 0;  /* +0x54: anim frame */
    data[22] = 0;  /* +0x58: anim timer */
    data[23] = 0;  /* +0x5c: anim flags */
}

/* PlayerAnim_Set @ 0x02010094 (216 bytes)
 * Sets player animation.
 * Args: r0=player, r1=anim_id */
void PlayerAnim_Set(void *player, u32 anim_id) {
    u32 *data = (u32*)player;
    
    /* Set new animation */
    data[20] = anim_id;  /* +0x50: current anim */
    data[21] = 0;        /* +0x54: reset frame */
    data[22] = 0;        /* +0x58: reset timer */
}

/* PlayerAnim_Update @ 0x02010170 (264 bytes)
 * Updates player animation.
 * Args: r0=player */
void PlayerAnim_Update(void *player) {
    u32 *data = (u32*)player;
    u32 anim_id = data[20];
    u32 frame = data[21];
    u32 timer = data[22];
    
    /* Get animation info */
    u32 *anim_info = (u32*)(0x20c0000 + (anim_id * 16));
    u32 num_frames = anim_info[0];
    u32 frame_duration = anim_info[1];
    
    /* Increment timer */
    data[22] = timer + 1;
    
    /* Check for frame advance */
    if (timer >= frame_duration) {
        /* Advance frame */
        data[21] = (frame + 1) % num_frames;
        data[22] = 0;  /* Reset timer */
    }
}

/* PlayerAnim_Get @ 0x02010278
 * Returns current animation ID.
 * Args: r0=player
 * Returns: animation ID */
u32 PlayerAnim_Get(void *player) {
    u32 *data = (u32*)player;
    return data[20];
}

/* PlayerAnim_GetFrame @ 0x020102a0
 * Returns current animation frame.
 * Args: r0=player
 * Returns: frame */
u32 PlayerAnim_GetFrame(void *player) {
    u32 *data = (u32*)player;
    return data[21];
}

/* PlayerAnim_IsComplete @ 0x020102c0
 * Returns whether animation is complete.
 * Args: r0=player
 * Returns: 1 if complete, 0 otherwise */
s32 PlayerAnim_IsComplete(void *player) {
    u32 *data = (u32*)player;
    u32 anim_id = data[20];
    u32 frame = data[21];
    
    /* Get animation info */
    u32 *anim_info = (u32*)(0x20c0000 + (anim_id * 16));
    u32 num_frames = anim_info[0];
    u32 loop = anim_info[2];
    
    /* Check if at last frame and not looping */
    if (frame >= num_frames - 1 && !loop) {
        return 1;
    }
    
    return 0;
}

/* PlayerAnim_Force @ 0x02010340
 * Forces animation frame.
 * Args: r0=player, r1=frame */
void PlayerAnim_Force(void *player, u32 frame) {
    u32 *data = (u32*)player;
    data[21] = frame;
    data[22] = 0;
}

/* PlayerAnim_Is @ 0x02010380
 * Checks if player has specific animation.
 * Args: r0=player, r1=anim_id
 * Returns: 1 if matching, 0 otherwise */
s32 PlayerAnim_Is(void *player, u32 anim_id) {
    u32 *data = (u32*)player;
    return data[20] == anim_id;
}
