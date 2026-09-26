/* Sprite animation functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

/* SpriteAnim_Init @ 0x02005800 (148 bytes)
 * Initializes sprite animation.
 * Args: r0=anim_data, r1=anim_id */
void SpriteAnim_Init(void *anim_data, u32 anim_id) {
    u32 *data = (u32*)anim_data;
    
    /* Set animation parameters */
    data[0] = anim_id;     /* +0x00: animation ID */
    data[1] = 0;           /* +0x04: current frame */
    data[2] = 0;           /* +0x08: frame timer */
    data[3] = 0;           /* +0x0c: flags */
}

/* SpriteAnim_Update @ 0x02005894 (216 bytes)
 * Updates sprite animation.
 * Args: r0=anim_data */
void SpriteAnim_Update(void *anim_data) {
    u32 *data = (u32*)anim_data;
    u32 anim_id = data[0];
    u32 frame = data[1];
    u32 timer = data[2];
    
    /* Get animation info */
    u32 *anim_info = (u32*)(0x2080000 + (anim_id * 16));
    u32 num_frames = anim_info[0];
    u32 frame_duration = anim_info[1];
    
    /* Increment timer */
    data[2] = timer + 1;
    
    /* Check for frame advance */
    if (timer >= frame_duration) {
        /* Advance frame */
        data[1] = (frame + 1) % num_frames;
        data[2] = 0;  /* Reset timer */
    }
}

/* SpriteAnim_SetFrame @ 0x02005970 (264 bytes)
 * Sets sprite animation frame.
 * Args: r0=anim_data, r1=frame */
void SpriteAnim_SetFrame(void *anim_data, u32 frame) {
    u32 *data = (u32*)anim_data;
    u32 anim_id = data[0];
    
    /* Get animation info */
    u32 *anim_info = (u32*)(0x2080000 + (anim_id * 16));
    u32 num_frames = anim_info[0];
    
    /* Clamp frame */
    if (frame >= num_frames) {
        frame = num_frames - 1;
    }
    
    /* Set frame */
    data[1] = frame;
    data[2] = 0;  /* Reset timer */
}

/* SpriteAnim_GetFrame @ 0x02005a78
 * Returns current animation frame.
 * Args: r0=anim_data
 * Returns: current frame */
u32 SpriteAnim_GetFrame(void *anim_data) {
    u32 *data = (u32*)anim_data;
    return data[1];
}

/* SpriteAnim_IsComplete @ 0x02005aa0
 * Returns whether animation has completed.
 * Args: r0=anim_data
 * Returns: 1 if complete, 0 otherwise */
s32 SpriteAnim_IsComplete(void *anim_data) {
    u32 *data = (u32*)anim_data;
    u32 anim_id = data[0];
    u32 frame = data[1];
    
    /* Get animation info */
    u32 *anim_info = (u32*)(0x2080000 + (anim_id * 16));
    u32 num_frames = anim_info[0];
    u32 loop = anim_info[2];  /* +0x08: loop flag */
    
    /* Check if at last frame and not looping */
    if (frame >= num_frames - 1 && !loop) {
        return 1;
    }
    
    return 0;
}

/* SpriteAnim_Play @ 0x02005b00
 * Plays a new animation.
 * Args: r0=anim_data, r1=anim_id */
void SpriteAnim_Play(void *anim_data, u32 anim_id) {
    /* Set new animation */
    SpriteAnim_Init(anim_data, anim_id);
}
