/* Player debug functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md
 * Decompiled from ARM9 binary in player subsystem range. */

#include "nds_types.h"
#include "player.h"

extern PhysicsPlayer* g_current_player;

#define DEBUG_MAX_FLAGS 32

/* Debug state */
static u32 s_debug_flags[DEBUG_MAX_FLAGS / 32];
static u32 s_debug_active = 0;

extern void Debug_PrintString(s32 x, s32 y, const char* str);
extern int sprintf(char* str, const char* fmt, ...);

/* ======================================================================== */
/* PlayerDebug_Init                                                         */
/* Initializes the debug overlay system.                                    */
/* ======================================================================== */
void PlayerDebug_Init(void)
{
    u32 i;
    for (i = 0; i < DEBUG_MAX_FLAGS / 32; i++)
    {
        s_debug_flags[i] = 0;
    }
    s_debug_active = 0;
}

/* ======================================================================== */
/* PlayerDebug_Update                                                       */
/* Per-frame debug update: reads input for debug toggles.                   */
/* ======================================================================== */
void PlayerDebug_Update(void)
{
    /* Debug is typically enabled via debug keys or menu */
    /* No automatic update needed - driven by SetFlag/ClearFlag */
}

/* ======================================================================== */
/* PlayerDebug_Draw                                                         */
/* Draws debug information on screen.                                       */
/* ======================================================================== */
void PlayerDebug_Draw(void)
{
    PhysicsPlayer* p = g_current_player;
    char buf[64];

    if (p == NULL)
        return;

    /* Print player position */
    sprintf(buf, "POS X:%08X Y:%08X", (u32)p->pos_x, (u32)p->pos_y);
    Debug_PrintString(2, 2, buf);

    /* Print velocity */
    sprintf(buf, "VEL X:%04X Y:%04X", (u32)(u16)p->input_vel_x, (u32)(u16)p->input_vel_y);
    Debug_PrintString(2, 12, buf);

    /* Print state flags */
    sprintf(buf, "STATE:%08X", p->state_flags);
    Debug_PrintString(2, 22, buf);

    /* Print control flags */
    sprintf(buf, "CTRL:%04X", p->control_flags);
    Debug_PrintString(2, 32, buf);
}

/* ======================================================================== */
/* PlayerDebug_SetFlag                                                      */
/* Sets a debug flag by index.                                              */
/* Args: r0=index, r1=value (1=set, 0=clear)                               */
/* ======================================================================== */
void PlayerDebug_SetFlag(u32 index, u32 value)
{
    if (index >= DEBUG_MAX_FLAGS)
        return;

    if (value)
        s_debug_flags[index / 32] |= (1u << (index & 31));
    else
        s_debug_flags[index / 32] &= ~(1u << (index & 31));
}

/* ======================================================================== */
/* PlayerDebug_ClearFlag                                                    */
/* Clears a debug flag by index.                                            */
/* ======================================================================== */
void PlayerDebug_ClearFlag(u32 index)
{
    if (index >= DEBUG_MAX_FLAGS)
        return;

    s_debug_flags[index / 32] &= ~(1u << (index & 31));
}

/* ======================================================================== */
/* PlayerDebug_CheckFlag                                                    */
/* Checks if a debug flag is set.                                           */
/* Returns: 1 if flag is set, 0 otherwise.                                  */
/* ======================================================================== */
s32 PlayerDebug_CheckFlag(void)
{
    u32 index;
#if defined(__arm__) || defined(__thumb__)
    __asm__ volatile("mov %0, r0" : "=r"(index));
#else
    index = 0;
#endif
    if (index >= DEBUG_MAX_FLAGS)
        return 0;
    return (s_debug_flags[index / 32] >> (index & 31)) & 1;
}

/* ======================================================================== */
/* PlayerDebug_PrintPosition                                                */
/* Prints player position to debug console.                                 */
/* ======================================================================== */
void PlayerDebug_PrintPosition(void)
{
    PhysicsPlayer* p = g_current_player;
    char buf[48];

    if (p == NULL)
        return;

    sprintf(buf, "P[%d,%d]", (s32)(p->pos_x >> 8), (s32)(p->pos_y >> 8));
    Debug_PrintString(2, 42, buf);
}
