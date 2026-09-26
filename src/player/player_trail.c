/* Player trail functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md
 * Decompiled from ARM9 binary in player subsystem range. */

#include "nds_types.h"
#include "player.h"

extern PhysicsPlayer* g_current_player;

#define TRAIL_MAX_POINTS 32
#define TRAIL_SPACING    8

/* Trail point structure */
typedef struct {
    s32 x;
    s32 y;
    u32 timer;
    u32 active;
} TrailPoint;

/* Trail state */
static TrailPoint s_trail_points[TRAIL_MAX_POINTS];
static u32 s_trail_count = 0;
static u32 s_trail_write_idx = 0;
static u32 s_trail_active = 0;

extern void OAM_SetSprite(u32 id, s32 x, s32 y, u32 tile, u32 attr);
extern u32 g_oam_count;

/* Forward declarations */
void PlayerTrail_Add(s32 x, s32 y, u32 param);

/* ======================================================================== */
/* PlayerTrail_Init                                                         */
/* Initializes the trail system, clears all points.                         */
/* ======================================================================== */
void PlayerTrail_Init(void)
{
    u32 i;

    s_trail_count = 0;
    s_trail_write_idx = 0;
    s_trail_active = 1;

    for (i = 0; i < TRAIL_MAX_POINTS; i++)
    {
        s_trail_points[i].x = 0;
        s_trail_points[i].y = 0;
        s_trail_points[i].timer = 0;
        s_trail_points[i].active = 0;
    }
}

/* ======================================================================== */
/* PlayerTrail_Update                                                       */
/* Per-frame trail update: decrements timers, fades out old points.         */
/* ======================================================================== */
void PlayerTrail_Update(void)
{
    u32 i;
    PhysicsPlayer* p = g_current_player;

    if (!s_trail_active)
        return;

    for (i = 0; i < TRAIL_MAX_POINTS; i++)
    {
        if (s_trail_points[i].active)
        {
            if (s_trail_points[i].timer > 0)
            {
                s_trail_points[i].timer--;
            }
            else
            {
                s_trail_points[i].active = 0;
                s_trail_count--;
            }
        }
    }

    /* Add new trail point from player position */
    if (p != NULL)
    {
        s32 px = p->pos_x >> 8;
        s32 py = p->pos_y >> 8;

        /* Only add if moved far enough from last point */
        if (s_trail_write_idx > 0)
        {
            u32 last = (s_trail_write_idx - 1) % TRAIL_MAX_POINTS;
            s32 dx = px - s_trail_points[last].x;
            s32 dy = py - s_trail_points[last].y;
            if (dx < 0) dx = -dx;
            if (dy < 0) dy = -dy;
            if (dx < TRAIL_SPACING && dy < TRAIL_SPACING)
                return;
        }

        PlayerTrail_Add(px, py, 20);
    }
}

/* ======================================================================== */
/* PlayerTrail_Add                                                          */
/* Adds a new trail point.                                                  */
/* Args: x, y = position in screen coords, param = lifetime in frames      */
/* ======================================================================== */
void PlayerTrail_Add(s32 x, s32 y, u32 param)
{
    u32 idx;

    if (!s_trail_active)
        return;

    idx = s_trail_write_idx % TRAIL_MAX_POINTS;

    /* If overwriting an active point, decrement count */
    if (s_trail_points[idx].active)
    {
        s_trail_count--;
    }

    s_trail_points[idx].x = x;
    s_trail_points[idx].y = y;
    s_trail_points[idx].timer = param;
    s_trail_points[idx].active = 1;
    s_trail_count++;

    s_trail_write_idx = (s_trail_write_idx + 1) % TRAIL_MAX_POINTS;
}

/* ======================================================================== */
/* PlayerTrail_Draw                                                         */
/* Draws all active trail points as OAM sprites.                            */
/* ======================================================================== */
void PlayerTrail_Draw(void)
{
    u32 i;

    if (!s_trail_active)
        return;

    for (i = 0; i < TRAIL_MAX_POINTS; i++)
    {
        if (s_trail_points[i].active && s_trail_points[i].timer > 0)
        {
            /* Alpha based on remaining timer */
            u32 alpha = (s_trail_points[i].timer * 16) / 20;
            if (alpha > 15) alpha = 15;

            OAM_SetSprite(
                g_oam_count,
                s_trail_points[i].x,
                s_trail_points[i].y,
                0x300,
                (alpha << 12) | 0x0
            );
            g_oam_count++;
        }
    }
}

/* ======================================================================== */
/* PlayerTrail_Clear                                                        */
/* Clears all trail points.                                                 */
/* ======================================================================== */
void PlayerTrail_Clear(void)
{
    u32 i;

    for (i = 0; i < TRAIL_MAX_POINTS; i++)
    {
        s_trail_points[i].active = 0;
        s_trail_points[i].timer = 0;
    }

    s_trail_count = 0;
    s_trail_write_idx = 0;
}

/* ======================================================================== */
/* PlayerTrail_GetCount                                                     */
/* Returns the number of active trail points.                               */
/* ======================================================================== */
u32 PlayerTrail_GetCount(void)
{
    return s_trail_count;
}

/* ======================================================================== */
/* PlayerTrail_GetPoint                                                     */
/* Retrieves a specific trail point.                                        */
/* Args: r0=index, r1=out_x ptr, r2=out_y ptr                              */
/* ======================================================================== */
void PlayerTrail_GetPoint(u32 index, s32* out_x, s32* out_y)
{
    if (index >= TRAIL_MAX_POINTS)
    {
        if (out_x) *out_x = 0;
        if (out_y) *out_y = 0;
        return;
    }

    if (out_x) *out_x = s_trail_points[index].x;
    if (out_y) *out_y = s_trail_points[index].y;
}
