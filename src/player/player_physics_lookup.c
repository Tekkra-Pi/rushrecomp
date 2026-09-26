/* Physics lookup and utility functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md line 313-421
 * Decompiled from ARM9 binary in player subsystem range. */

#include "nds_types.h"
#include "player.h"

extern PhysicsPlayer* g_current_player;

/* External lookup tables */
extern const s8 g_accel_scale_table[];
extern const s16 g_slope_table[];
extern const s8 g_drift_table[];

/* ======================================================================== */
/* Player_LookupAccelScale @ 0x0206b0bc+                                    */
/* Looks up acceleration scale from table based on player speed.            */
/* Args: r0=speed_index                                                     */
/* Returns: scaled acceleration value                                       */
/* ======================================================================== */
s32 Player_LookupAccelScale(void)
{
    PhysicsPlayer* p = g_current_player;
    s32 speed;
    s32 idx;

    /* Compute speed index from current velocity */
    speed = p->input_vel_x;
    if (speed < 0) speed = -speed;

    /* Clamp index to table bounds (0-31) */
    idx = speed >> 8;
    if (idx < 0) idx = 0;
    if (idx > 31) idx = 31;

    return (s32)g_accel_scale_table[idx];
}

/* ======================================================================== */
/* Player_LookupSlopeTable @ 0x0206b1bc+                                    */
/* Looks up slope velocity contribution from table based on angle.          */
/* Returns: slope velocity component (fixed-point 8.8)                      */
/* ======================================================================== */
s32 Player_LookupSlopeTable(void)
{
    PhysicsPlayer* p = g_current_player;
    s32 angle;
    s32 idx;
    s32 result;

    /* Get current angle */
    angle = ((u8*)p)[1];  /* direction byte */
    if (angle < 0) angle = -angle;

    /* Index into slope table (each entry is 4 bytes: sin, cos pair) */
    idx = (angle * 2) & 0xFF;
    result = (s32)g_slope_table[idx];

    return result;
}

/* ======================================================================== */
/* Player_DriftTableLookup @ 0x0206b244+                                    */
/* Looks up drift table value based on action timer high nibble.            */
/* The high nibble of action_timer is divided by 0x20 to get index.        */
/* Returns: drift velocity modifier                                         */
/* ======================================================================== */
s32 Player_DriftTableLookup(void)
{
    PhysicsPlayer* p = g_current_player;
    s32 idx;
    s32 val_x, val_y;

    /* Get drift index from action_timer high nibble */
    idx = (p->action_timer & 0xF0) >> 5;

    /* Look up drift values (pairs of s8) */
    val_x = (s32)g_drift_table[idx * 2];
    val_y = (s32)g_drift_table[idx * 2 + 1];

    /* Apply to player's slope acceleration bytes */
    *(s8*)((u8*)p + 6) += (s8)val_x;
    *(s8*)((u8*)p + 7) += (s8)val_y;

    return val_x;
}
