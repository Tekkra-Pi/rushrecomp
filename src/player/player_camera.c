/* Player camera functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md
 * Decompiled from ARM9 binary in player subsystem range. */

#include "nds_types.h"
#include "player.h"

extern PhysicsPlayer* g_current_player;

/* Camera state */
static s32 s_cam_target_x = 0;
static s32 s_cam_target_y = 0;
static s32 s_cam_scroll_x = 0;
static s32 s_cam_scroll_y = 0;
static s32 s_cam_shake_x = 0;
static s32 s_cam_shake_y = 0;
static u32 s_cam_shake_timer = 0;
static u32 s_cam_lock = 0;
static u32 s_cam_lock_x = 0;
static u32 s_cam_lock_y = 0;
static s32 s_cam_bounds_left = 0;
static s32 s_cam_bounds_right = 0;
static s32 s_cam_bounds_top = 0;
static s32 s_cam_bounds_bottom = 0;

extern s32 g_camera_x;
extern s32 g_camera_y;
extern s32 g_stage_width;
extern s32 g_stage_height;

#define CAM_DEADZONE_X  64
#define CAM_DEADZONE_Y  32
#define CAM_FOLLOW_SPEED 0x10

/* ======================================================================== */
/* PlayerCamera_Init                                                        */
/* Initializes camera state with default values.                            */
/* ======================================================================== */
void PlayerCamera_Init(void)
{
    PhysicsPlayer* p = g_current_player;

    s_cam_target_x = 0;
    s_cam_target_y = 0;
    s_cam_scroll_x = 0;
    s_cam_scroll_y = 0;
    s_cam_shake_x = 0;
    s_cam_shake_y = 0;
    s_cam_shake_timer = 0;
    s_cam_lock = 0;
    s_cam_lock_x = 0;
    s_cam_lock_y = 0;

    /* Set initial bounds */
    s_cam_bounds_left = 0;
    s_cam_bounds_right = g_stage_width;
    s_cam_bounds_top = 0;
    s_cam_bounds_bottom = g_stage_height;

    if (p != NULL)
    {
        s_cam_target_x = p->pos_x >> 8;
        s_cam_target_y = p->pos_y >> 8;
        s_cam_scroll_x = s_cam_target_x - 128;
        s_cam_scroll_y = s_cam_target_y - 96;
    }
}

/* ======================================================================== */
/* PlayerCamera_Update                                                      */
/* Per-frame camera update: follows player with deadzone, applies shake.    */
/* ======================================================================== */
void PlayerCamera_Update(void)
{
    PhysicsPlayer* p = g_current_player;
    s32 player_x, player_y;
    s32 diff_x, diff_y;

    if (p == NULL)
        return;

    player_x = p->pos_x >> 8;
    player_y = p->pos_y >> 8;

    if (s_cam_lock)
    {
        /* Camera locked to fixed position */
        s_cam_scroll_x = (s32)s_cam_lock_x;
        s_cam_scroll_y = (s32)s_cam_lock_y;
    }
    else
    {
        /* Follow player with deadzone */
        diff_x = player_x - (s_cam_scroll_x + 128);
        diff_y = player_y - (s_cam_scroll_y + 96);

        if (diff_x > CAM_DEADZONE_X)
            s_cam_scroll_x += (diff_x - CAM_DEADZONE_X) >> 2;
        else if (diff_x < -CAM_DEADZONE_X)
            s_cam_scroll_x += (diff_x + CAM_DEADZONE_X) >> 2;

        if (diff_y > CAM_DEADZONE_Y)
            s_cam_scroll_y += (diff_y - CAM_DEADZONE_Y) >> 2;
        else if (diff_y < -CAM_DEADZONE_Y)
            s_cam_scroll_y += (diff_y + CAM_DEADZONE_Y) >> 2;
    }

    /* Clamp to level bounds */
    if (s_cam_scroll_x < s_cam_bounds_left)
        s_cam_scroll_x = s_cam_bounds_left;
    if (s_cam_scroll_x > s_cam_bounds_right - 256)
        s_cam_scroll_x = s_cam_bounds_right - 256;
    if (s_cam_scroll_y < s_cam_bounds_top)
        s_cam_scroll_y = s_cam_bounds_top;
    if (s_cam_scroll_y > s_cam_bounds_bottom - 192)
        s_cam_scroll_y = s_cam_bounds_bottom - 192;

    /* Apply shake offset */
    s_cam_shake_x = 0;
    s_cam_shake_y = 0;
    if (s_cam_shake_timer > 0)
    {
        s_cam_shake_timer--;
        s_cam_shake_x = ((s_cam_shake_timer & 1) ? 2 : -2);
        s_cam_shake_y = ((s_cam_shake_timer & 2) ? 1 : -1);
    }

    /* Write final scroll values */
    g_camera_x = s_cam_scroll_x + s_cam_shake_x;
    g_camera_y = s_cam_scroll_y + s_cam_shake_y;
}

/* ======================================================================== */
/* PlayerCamera_SetTarget                                                   */
/* Sets a camera target position.                                           */
/* Args: r0=index (0=x, 1=y), r1=value                                     */
/* ======================================================================== */
void PlayerCamera_SetTarget(u32 index, u32 value)
{
    if (index == 0)
        s_cam_target_x = (s32)value;
    else
        s_cam_target_y = (s32)value;
}

/* ======================================================================== */
/* PlayerCamera_GetTarget                                                   */
/* Gets the current camera target position.                                 */
/* ======================================================================== */
void PlayerCamera_GetTarget(s32* out_x, s32* out_y)
{
    if (out_x) *out_x = s_cam_target_x;
    if (out_y) *out_y = s_cam_target_y;
}

/* ======================================================================== */
/* PlayerCamera_GetScroll                                                   */
/* Gets the current scroll position.                                        */
/* ======================================================================== */
void PlayerCamera_GetScroll(s32* out_x, s32* out_y)
{
    if (out_x) *out_x = s_cam_scroll_x;
    if (out_y) *out_y = s_cam_scroll_y;
}

/* ======================================================================== */
/* PlayerCamera_SetScroll                                                   */
/* Sets the scroll position directly.                                       */
/* Args: r0=index (0=x, 1=y), r1=value                                     */
/* ======================================================================== */
void PlayerCamera_SetScroll(u32 index, u32 value)
{
    if (index == 0)
        s_cam_scroll_x = (s32)value;
    else
        s_cam_scroll_y = (s32)value;
}

/* ======================================================================== */
/* PlayerCamera_Shake                                                       */
/* Triggers camera shake for given duration.                                */
/* Args: r0=duration in frames                                              */
/* ======================================================================== */
void PlayerCamera_Shake(u32 duration)
{
    s_cam_shake_timer = duration;
}

/* ======================================================================== */
/* PlayerCamera_Lock                                                        */
/* Locks camera to current position.                                        */
/* ======================================================================== */
void PlayerCamera_Lock(void)
{
    s_cam_lock = 1;
    s_cam_lock_x = s_cam_scroll_x;
    s_cam_lock_y = s_cam_scroll_y;
}

/* ======================================================================== */
/* PlayerCamera_Unlock                                                      */
/* Unlocks camera, resuming follow mode.                                    */
/* ======================================================================== */
void PlayerCamera_Unlock(void)
{
    s_cam_lock = 0;
}

/* ======================================================================== */
/* PlayerCamera_GetBounds                                                   */
/* Gets the camera bounds.                                                  */
/* ======================================================================== */
void PlayerCamera_GetBounds(s32* out_left, s32* out_right, s32* out_top, s32* out_bottom)
{
    if (out_left)   *out_left   = s_cam_bounds_left;
    if (out_right)  *out_right  = s_cam_bounds_right;
    if (out_top)    *out_top    = s_cam_bounds_top;
    if (out_bottom) *out_bottom = s_cam_bounds_bottom;
}
