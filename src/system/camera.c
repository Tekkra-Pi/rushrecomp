/* Camera system functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"
#include "player.h"

/* Camera state at 0x22c45c0 */
extern s32 g_camera_x;
extern s32 g_camera_y;
extern s32 g_camera_target_x;
extern s32 g_camera_target_y;

/* Camera_FollowPlayer @ 0x020643b4 (180 bytes)
 * Updates camera to follow player.
 * Args: r0=player */
void Camera_FollowPlayer(PhysicsPlayer *player) {
    /* Calculate target camera position */
    g_camera_target_x = player->pos_x - 128;  /* Center on player */
    g_camera_target_y = player->pos_y - 96;
    
    /* Smooth follow */
    g_camera_x += (g_camera_target_x - g_camera_x) >> 4;
    g_camera_y += (g_camera_target_y - g_camera_y) >> 4;
    
    /* Clamp to level bounds */
    if (g_camera_x < 0) g_camera_x = 0;
    if (g_camera_y < 0) g_camera_y = 0;
}

/* Camera_Update @ 0x020644ac (92 bytes)
 * Updates camera position each frame. */
void Camera_Update(void) {
    /* Apply camera offset to BG layers */
    REG_BG0HOFS = -g_camera_x & 0x1ff;
    REG_BG0VOFS = -g_camera_y & 0x1ff;
    REG_BG1HOFS = -g_camera_x & 0x1ff;
    REG_BG1VOFS = -g_camera_y & 0x1ff;
}

/* Camera_Init @ 0x02064508 (44 bytes)
 * Initializes camera at specified position.
 * Args: r0=x, r1=y */
void Camera_Init(s32 x, s32 y) {
    g_camera_x = x;
    g_camera_y = y;
    g_camera_target_x = x;
    g_camera_target_y = y;
}
