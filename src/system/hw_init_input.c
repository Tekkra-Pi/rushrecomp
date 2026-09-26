/* Early ARM9 hardware init - input. */

#include "nds_types.h"
#include "input.h"

extern InputSubsystem g_input_subsystem;

void HwInit_Input(void) {
    extern void Input_SubsysInit(void);
    Input_SubsysInit();
    memset(&g_input_subsystem, 0, sizeof(InputSubsystem));
}

void HwInit_Touchscreen(void) {
    /* Initialize touchscreen controller */
    *(vu16*)0x04000004 = 0;
    g_input_subsystem.contact_mask = 0;
}
