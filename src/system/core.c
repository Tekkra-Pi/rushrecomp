/* Core system functions.
 * Evidence: CONFIRMED-STATIC from re-analysis.md */

#include "nds_types.h"

extern u32 g_system_flags;
extern u32 g_game_paused;
extern u32 g_player_flags;

extern void Func_0204edc0(void);
extern void Func_0202ff74(void);
extern void Func_02010590(void);
extern u32 Input_IsReady(void);
extern void TouchState_Update(void);
extern void Subsystem_PerFrame(void);
extern void PostEntityWalk(void);
extern void Overlay_TransitionStep(void);
extern void Boot_ResetStateMachine(void);
extern void Func_0202ff80(void);
extern void ObjectList_PerFrameWalker(u32 phase);
extern void Overlay_ScheduleFromInput(void);
extern void Overlay_SwapActive(void);
extern void Func_0203005c(void);
extern void System_Halt(void);
extern void Entity_Init(void);
extern void ObjectList_Init(void);
extern void Controller_Init(void);
extern void Sound_Init(void);
extern void Heap_Init(void);
extern void Display_Init(void);
extern void Overlay_Init(void);
extern DmaChannelEntry g_dma_channels[];
extern TaskEntry g_tasks[];
extern CallbackEntry g_callbacks[];

void IRQ6_Channel6_NoopStub(u32 control_reg, u16 irq_flags, u32 status) {
    u32 channel_idx = ((u32)irq_flags << 16) >> 16;
    channel_idx = (channel_idx & 0x7F00) >> 8;
    (void)control_reg;
    (void)channel_idx;
    (void)status;
}

void Main_PerFrameLoop(void) {
    extern void Input_DebounceEdgeRead(void*, u16);
    extern u16 g_key_current, g_key_previous;
    extern u32 *g_system_flags_ptr;

    System_Init();
    Func_0204edc0();

    while (1) {
        if (Input_IsReady()) {
            Func_0202ff74();
            Func_02010590();
        }
        if (!(g_system_flags & 0x40)) {
            u16 pressed = (g_key_current | g_key_previous) ^ g_player_flags;
            pressed = (pressed & (u16)g_player_flags);
            Input_DebounceEdgeRead((void*)0x22503e0, pressed);
        }
        TouchState_Update();
        Subsystem_PerFrame();
        if (!(g_system_flags & 0x10)) {
            if ((g_key_current & 0x30C) == 0x30C) {
                extern u32 g_game_paused;
                g_game_paused = !g_game_paused;
            }
        }
        Boot_ResetStateMachine();
        Func_0202ff80();
        Overlay_TransitionStep();
        Func_02036fb8();
        PostEntityWalk();
        ObjectList_PerFrameWalker(0);
        ObjectList_PerFrameWalker(1);
        {
            u16 key = g_key_current;
            if ((key & 0x000C) == 0x000C && !(g_system_flags & 0x8000000)) {
                Overlay_ScheduleFromInput();
            }
        }
        *g_system_flags_ptr = g_system_flags;
        g_system_flags &= ~1;
        System_Halt();
        g_system_flags = *g_system_flags_ptr;
        if (g_system_flags & 0x100) {
            Overlay_SwapActive();
        }
        Func_0203005c();
        {
            u32 f = g_system_flags & ~0x80000000;
            g_system_flags = (f & 8) ? (f | 0x80000000) : f;
        }
    }
}

void System_Init(void) {
    static u32 done = 0;
    u32 i;
    if (done) return;
    done = 1;
    g_system_flags = 0;
    g_game_paused = 0;
    g_player_flags = 0;
    Heap_Init();
    Display_Init();
    for (i = 0; i < 4; i++) {
        g_dma_channels[i].active = 0;
        g_dma_channels[i].callback = NULL;
    }
    for (i = 0; i < 16; i++) {
        g_tasks[i].active = 0;
        g_tasks[i].func = NULL;
        g_tasks[i].priority = 0;
        g_tasks[i].paused = 0;
        g_callbacks[i].active = 0;
        g_callbacks[i].func = NULL;
        g_callbacks[i].type = 0;
    }
    Entity_Init();
    ObjectList_Init();
    Controller_Init();
    Sound_Init();
    Overlay_Init();
    g_system_flags |= 8;
}
