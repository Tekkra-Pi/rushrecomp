/* ─── PC Recomp: Main Entry Point ───
 * Initializes NDS address space emulation, then runs the game loop. */

#include <stdio.h>
#include <stdlib.h>

extern int nds_mmap_init(void);
extern void main_per_frame_loop(void);

int main(int argc, char **argv) {
    printf("=== Sonic Rush PC Recompilation ===\n");
    printf("Initializing NDS hardware emulation...\n");

    if (!nds_mmap_init()) {
        fprintf(stderr, "Failed to map NDS address space\n");
        return 1;
    }

    printf("Entering game loop (Ctrl+C to exit)...\n");

    /* This never returns in normal operation */
    main_per_frame_loop();

    return 0;
}
