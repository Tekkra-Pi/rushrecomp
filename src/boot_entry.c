/* ─── ARM9 Boot Entry ───
 * Placed at 0x02000000. Initializes BSS, stack, and jumps to main loop. */

#include "nds_types.h"

#if defined(__arm__) || defined(__thumb__)
extern u32 __bss_start[];
extern u32 __bss_end[];
#endif

extern void main_per_frame_loop(void);

void _start(void) {
#if defined(__arm__) || defined(__thumb__)
    /* Zero BSS (ARM only — PC OS handles this) */
    for (u32 *p = __bss_start; p < __bss_end; p++) {
        *p = 0;
    }
#endif
    main_per_frame_loop();
}
