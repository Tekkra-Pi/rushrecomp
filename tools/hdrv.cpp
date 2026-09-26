// Headless Sonic Rush runtime inspection driver.
// Links against libdesmume.a. Loads the ROM, steps frames (with optional
// input injection), and dumps selected ARM9 main-RAM words each N frames.
// Usage: hdrv <rom.nds> <out.csv> <totalframes> [dumpstep] [--input hex]
#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include "NDSSystem.h"
#include "armcpu.h"
#include "MMU.h"
#include "slot2.h"
#include "GPU.h"
#include "SPU.h"
#include "driver.h"
#include "interface.h"
#include "render3D.h"
#include "rasterize.h"

// ── Game-specific RAM addresses (Sonic Rush NDS USA) ──
// Mode / state
static const u32 ADDR_MODE_PTR      = 0x231f050;
static const u32 ADDR_PREV_KEY      = 0x27fffa8;
static const u32 ADDR_ATTRACT       = 0x22C4EE4;
static const u32 ADDR_TOPMODE       = 0x22C4EEC;
static const u32 ADDR_ZONE          = 0x22C4560;
static const u32 ADDR_GATE_POKE     = 0x22C4580;

// Entity list head
static const u32 ADDR_ENTITY_HEAD   = 0x22B4574;

// Touch state (0x28 bytes)
static const u32 ADDR_TOUCH_STATE   = 0x22B65B4;

// Controller ring
static const u32 ADDR_CTRL_RING     = 0x207F02C;

// Overlay state
static const u32 ADDR_OV_ACTIVE     = 0x2087cb4;
static const u32 ADDR_OV_REQUEST    = 0x2087cbc;
static const u32 ADDR_OV_LOADED     = 0x2087cc8;
static const u32 ADDR_OV_CALLBACK   = 0x2087cb0;

// NDS hardware registers
static const u32 REG_POWCNT  = 0x04000304;
static const u32 REG_DISPSTAT = 0x04000004;
static const u32 REG_VCOUNT  = 0x04000006;
static const u32 REG_DISPCNT = 0x04000000;
static const u32 REG_KEYIN   = 0x04000130;
static const u32 REG_KEYCNT  = 0x04000136;

void* createThread_gdb(void (*thread_function)(void* data), void* thread_data) {
    return (void*)SDL_CreateThread((int (*)(void*))thread_function, "gdb-stub", thread_data);
}
void joinThread_gdb(void* thread_handle) {
    int ignore;
    SDL_WaitThread((SDL_Thread*)thread_handle, &ignore);
}

struct DumpRegion {
    u32 start;
    u32 len;
    const char* out;
};

struct DumpFrameRegion {
    int frame;
    u32 start;
    u32 len;
    const char* out;
};

struct ScriptEvent {
    int frame;
    int type; // 0=keypad set, 1=touch tap, 2=touch hold, 3=touch release, 4=poke32
    unsigned long v1, v2;
};

static std::vector<DumpRegion> g_dumps;
static std::vector<DumpFrameRegion> g_dumpFrames;
static std::vector<ScriptEvent> g_script;
static int g_scriptIdx = 0;
static unsigned long g_curMask = 0;
static bool g_touchActive = false;
static int g_touchHoldEnd = -1;

static void applyScript(int f)
{
    while (g_scriptIdx < (int)g_script.size() && g_script[g_scriptIdx].frame <= f) {
        ScriptEvent& e = g_script[g_scriptIdx];
        if (e.type == 0) {
            g_curMask = e.v1;
        } else if (e.type == 1) {
            NDS_setTouchPos(e.v1, e.v2);
            g_touchActive = true;
            g_touchHoldEnd = f + 2;
        } else if (e.type == 2) {
            NDS_setTouchPos(e.v1, e.v2);
            g_touchActive = true;
            g_touchHoldEnd = -1; // hold until released
        } else if (e.type == 3) {
            NDS_releaseTouch();
            g_touchActive = false;
            g_touchHoldEnd = -1;
        } else if (e.type == 4) {
            _MMU_write32<ARMCPU_ARM9>(e.v1, e.v2);
        }
        g_scriptIdx++;
    }
    if (g_touchActive && g_touchHoldEnd >= 0 && f >= g_touchHoldEnd) {
        NDS_releaseTouch();
        g_touchActive = false;
        g_touchHoldEnd = -1;
    }
}

volatile bool execute = false;
SoundInterface_struct* SNDCoreList[] = { &SNDDummy, NULL };
GPU3DInterface* core3DList[] = { &gpu3DNull, &gpu3DRasterize, NULL };

int main(int argc, char** argv)
{
    if (argc < 4) {
        fprintf(stderr, "usage: %s <rom> <out> <frames> [dumpstep] [--input hex]\n"
                        "          [--dump start len outraw ...]\n"
                        "          [--dump-frame N start len outraw ...]\n"
                        "          [--script file] [--shot dir]\n", argv[0]);
        return 2;
    }
    const char* rom = argv[1];
    const char* out = argv[2];
    int total = atoi(argv[3]);
    int dumpstep = (argc >= 5) ? atoi(argv[4]) : 60;
    unsigned long inputmask = 0;
    const char* shotdir = NULL;
    const char* scriptfile = NULL;
    bool gatepoke = false;
    for (int i = 5; i < argc; i++) {
        if (!strcmp(argv[i], "--input") && i + 1 < argc) {
            inputmask = strtoul(argv[i+1], NULL, 16);
        }
        if (!strcmp(argv[i], "--gatepoke")) {
            gatepoke = true;
        }
        if (!strcmp(argv[i], "--shot") && i + 1 < argc) {
            shotdir = argv[i+1];
        }
        if (!strcmp(argv[i], "--script") && i + 1 < argc) {
            scriptfile = argv[i+1];
        }
        if (!strcmp(argv[i], "--dump") && i + 3 < argc) {
            DumpRegion r;
            r.start = strtoul(argv[i+1], NULL, 16);
            r.len = strtoul(argv[i+2], NULL, 16);
            r.out = argv[i+3];
            g_dumps.push_back(r);
            i += 3;
        }
        if (!strcmp(argv[i], "--dump-frame") && i + 4 < argc) {
            DumpFrameRegion r;
            r.frame = atoi(argv[i+1]);
            r.start = strtoul(argv[i+2], NULL, 16);
            r.len = strtoul(argv[i+3], NULL, 16);
            r.out = argv[i+4];
            g_dumpFrames.push_back(r);
            i += 4;
        }
    }

    if (scriptfile) {
        FILE* sf = fopen(scriptfile, "r");
        if (!sf) { fprintf(stderr, "cannot read %s\n", scriptfile); return 1; }
        char line[256];
        int lineno = 0;
        int prevFrame = -1;
        bool sorted = true;
        while (fgets(line, sizeof(line), sf)) {
            lineno++;
            char* s = strchr(line, '#');
            if (s) *s = 0;
            // skip blank lines
            char* p = line;
            while (*p == ' ' || *p == '\t') p++;
            if (*p == '\n' || *p == '\0') continue;
            int fr = 0; char type = '\0';
            unsigned long v1 = 0, v2 = 0;
            int fields = sscanf(line, "%d %c %lx %lx", &fr, &type, &v1, &v2);
            int eventType = (type == 'k') ? 0 : (type == 't') ? 1 : (type == 'h') ? 2 : (type == 'r') ? 3 : (type == 'p') ? 4 : -1;
            int requiredFields = (eventType == 3) ? 2 : (eventType == 0 ? 3 : 4);
            if (eventType >= 0 && fields >= requiredFields) {
                ScriptEvent e;
                e.frame = fr;
                e.type = eventType;
                e.v1 = v1; e.v2 = v2;
                g_script.push_back(e);
                if (fr < prevFrame) {
                    sorted = false;
                    fprintf(stderr, "script:%d: WARNING: frame %d < previous frame %d (events should be sorted)\n",
                            lineno, fr, prevFrame);
                }
                prevFrame = fr;
            } else if (fields >= 2) {
                fprintf(stderr, "script:%d: invalid event: %s", lineno, line);
                fclose(sf);
                return 1;
            } else if (fields >= 1) {
                fprintf(stderr, "script:%d: unknown event type '%c': %s", lineno, type, line);
                fclose(sf);
                return 1;
            }
        }
        fclose(sf);
        if (!sorted) {
            fprintf(stderr, "script: WARNING: events are not in frame order; some may be missed\n");
        }
        printf("script: %d events\n", (int)g_script.size());
        g_curMask = inputmask;
    }

    // Minimal init (mirrors CLI before NDS_LoadROM).
    CommonSettings.use_jit = 1;
    CommonSettings.fwConfig.language = 1;
    slot2_Init();
    slot2_Change(NDS_SLOT2_NONE);
    class BaseDriver driver;
    ::driver = &driver;
    int err = NDS_Init();
    if (err < 0) { fprintf(stderr, "NDS_Init failed %d\n", err); return 1; }

    if (!GPU->Change3DRendererByID(RENDERID_SOFTRASTERIZER)) {
        fprintf(stderr, "3D renderer init failed\n");
    }

    err = NDS_LoadROM(rom);
    if (err < 0) { fprintf(stderr, "NDS_LoadROM failed %d\n", err); return 1; }
    execute = true;

    FILE* fp = fopen(out, "w");
    if (!fp) { fprintf(stderr, "cannot write %s\n", out); return 1; }
    fprintf(fp, "frame,powcnt,dispcnt,dispstat,vcount,keyin,keycnt,pc9,pc7,head,player,ctrl,gz0,gz1,gz2,p48,p4a,p4c,velx,vely,grav,term,yvel,flags,zone,z1c,z1d,zmod,touch0,touch4,touch8,touchc,touchflags,touch14,taskcb,taskidx,tasktable,taskentry0,taskentry1,attract,topmode,z20,prevkey,modep,mode,mstate,ovact,ovreq,ovload,ovcb0,zmode\n");

    for (int f = 0; f < total; f++) {
        // hold input during this frame
        if (scriptfile) {
            applyScript(f);
            inputmask = g_curMask;
        }
        if (inputmask) {
            bool A = (inputmask>>0)&1, B = (inputmask>>1)&1, S = (inputmask>>3)&1,
                 T = (inputmask>>2)&1, U = (inputmask>>6)&1, D = (inputmask>>7)&1,
                 L = (inputmask>>5)&1, R = (inputmask>>4)&1;
            NDS_setPad(R, L, D, U, T, S, B, A, false,false,false,false,false,false);
        } else {
            NDS_setPad(false,false,false,false,false,false,false,false,
                       false,false,false,false,false,false);
        }
        if (gatepoke) {
            _MMU_write32<ARMCPU_ARM9>(ADDR_GATE_POKE, 1);
        }
        NDS_beginProcessingInput();
        NDS_endProcessingInput();
        NDS_exec<false>();
        SPU_Emulate_user();

        if (shotdir && (dumpstep > 0) && (f % dumpstep == 0)) {
            u16* fb = GPU->GetDisplayInfo().masterNativeBuffer16;
            u32 vramNonZero = 0;
            for (int i = 0; i < 256 * 384; i++) {
                if (fb[i] != 0x7FFF) { vramNonZero++; }
            }
            fprintf(stderr, "frame %d: nonwhite=%u/%d\n", f, vramNonZero, 256*384);
            char tmppath[512];
            snprintf(tmppath, sizeof(tmppath), "%s/.hdrv_tmp.ppm", shotdir);
            FILE* fp2 = fopen(tmppath, "wb");
            fprintf(fp2, "P6\n%d %d\n255\n", 256, 384);
            for (int i = 0; i < 256 * 384; i++) {
                unsigned char b[3];
                b[0] = ((fb[i] >> 0) & 0x1f) << 3;
                b[1] = ((fb[i] >> 5) & 0x1f) << 3;
                b[2] = ((fb[i] >> 10) & 0x1f) << 3;
                fwrite(b, 1, 3, fp2);
            }
            fclose(fp2);
            char path[512];
            snprintf(path, sizeof(path), "%s/f%06d.ppm", shotdir, f);
            rename(tmppath, path);
        }
        if ((dumpstep > 0) && (f % dumpstep == 0)) {
            u32 powcnt = _MMU_read16<ARMCPU_ARM9>(REG_POWCNT);
            u32 dispstat = _MMU_read16<ARMCPU_ARM9>(REG_DISPSTAT);
            u32 vcount = _MMU_read16<ARMCPU_ARM9>(REG_VCOUNT);
            u32 dispcnt = _MMU_read16<ARMCPU_ARM9>(REG_DISPCNT);

            u32 keyin = _MMU_read16<ARMCPU_ARM9>(REG_KEYIN);
            u32 keycnt = _MMU_read16<ARMCPU_ARM9>(REG_KEYCNT);
            u32 pc9 = NDS_ARM9.instruct_adr;
            u32 pc7 = NDS_ARM7.instruct_adr;
            u32 head = _MMU_read32<ARMCPU_ARM9>(ADDR_ENTITY_HEAD);
            u32 player = head ? _MMU_read32<ARMCPU_ARM9>(head + 0x10) : 0;
            u32 ctrl = head ? _MMU_read32<ARMCPU_ARM9>(head + 0x14) : 0;
            u16 gz0 = player ? _MMU_read16<ARMCPU_ARM9>(player + 0x10) : 0;
            u16 gz1 = player ? _MMU_read16<ARMCPU_ARM9>(player + 0x14) : 0;
            u16 gz2 = player ? _MMU_read16<ARMCPU_ARM9>(player + 0x18) : 0;
            u32 p48 = player ? _MMU_read32<ARMCPU_ARM9>(player + 0x48) : 0;
            u32 p4a = player ? _MMU_read32<ARMCPU_ARM9>(player + 0x4a) : 0;
            u32 p4c = player ? _MMU_read32<ARMCPU_ARM9>(player + 0x4c) : 0;
            u16 velx = player ? _MMU_read16<ARMCPU_ARM9>(player + 0x28) : 0;
            u16 vely = player ? _MMU_read16<ARMCPU_ARM9>(player + 0x2a) : 0;
            u16 grav = player ? _MMU_read16<ARMCPU_ARM9>(player + 0x40) : 0;
            u16 term = player ? _MMU_read16<ARMCPU_ARM9>(player + 0x42) : 0;
            u16 yvel = player ? _MMU_read16<ARMCPU_ARM9>(player + 0x30) : 0;
            u16 flags = player ? _MMU_read16<ARMCPU_ARM9>(player + 0x58) : 0;
            u32 touch0 = _MMU_read32<ARMCPU_ARM9>(ADDR_TOUCH_STATE);
            u32 touch4 = _MMU_read32<ARMCPU_ARM9>(ADDR_TOUCH_STATE + 4);
            u32 touch8 = _MMU_read32<ARMCPU_ARM9>(ADDR_TOUCH_STATE + 8);
            u32 touchc = _MMU_read32<ARMCPU_ARM9>(ADDR_TOUCH_STATE + 0xc);
            u32 touchflags = _MMU_read32<ARMCPU_ARM9>(ADDR_TOUCH_STATE + 0x10);
            u32 touch14 = _MMU_read32<ARMCPU_ARM9>(ADDR_TOUCH_STATE + 0x14);
            u32 taskcb = _MMU_read32<ARMCPU_ARM9>(ADDR_CTRL_RING);
            u32 taskidx = _MMU_read16<ARMCPU_ARM9>(ADDR_CTRL_RING + 0xc);
            u32 tasktable = _MMU_read32<ARMCPU_ARM9>(ADDR_CTRL_RING + 0x10);
            u32 taskentry0 = tasktable ? _MMU_read32<ARMCPU_ARM9>(tasktable + taskidx * 8) : 0;
            u32 taskentry1 = tasktable ? _MMU_read32<ARMCPU_ARM9>(tasktable + taskidx * 8 + 4) : 0;
            u32 attract = _MMU_read16<ARMCPU_ARM9>(ADDR_ATTRACT);
            u32 topmode = _MMU_read16<ARMCPU_ARM9>(ADDR_TOPMODE);
            u32 zone = _MMU_read32<ARMCPU_ARM9>(ADDR_ZONE);
            u8 zone1c = zone ? _MMU_read08<ARMCPU_ARM9>(zone + 0x1c) : 0;
            u8 zone1d = zone ? _MMU_read08<ARMCPU_ARM9>(zone + 0x1d) : 0;
            u16 zmod = zone ? _MMU_read16<ARMCPU_ARM9>(zone + 0x10) : 0;
            u32 z20 = _MMU_read32<ARMCPU_ARM9>(ADDR_ZONE + 0x20);
            u32 prevkey = _MMU_read32<ARMCPU_ARM9>(ADDR_PREV_KEY);
            u32 modep = _MMU_read32<ARMCPU_ARM9>(ADDR_MODE_PTR);
            u32 mode = modep ? _MMU_read08<ARMCPU_ARM9>(modep + 0x00) : 0xff;
            u32 mstate = modep ? _MMU_read32<ARMCPU_ARM9>(modep + 0x10) : 0;
            u32 ovact = _MMU_read32<ARMCPU_ARM9>(ADDR_OV_ACTIVE);
            u32 ovreq = _MMU_read32<ARMCPU_ARM9>(ADDR_OV_REQUEST);
            u32 ovload = _MMU_read32<ARMCPU_ARM9>(ADDR_OV_LOADED);
            u32 ovcb0 = _MMU_read32<ARMCPU_ARM9>(ADDR_OV_CALLBACK);
            u32 zmode = _MMU_read16<ARMCPU_ARM9>(ADDR_ZONE + 0x10);
            fprintf(fp, "%d,0x%04x,0x%04x,0x%04x,0x%04x,0x%04x,0x%04x,0x%08x,0x%08x,0x%08x,0x%08x,0x%08x,0x%04x,0x%04x,0x%04x,"
                        "0x%08x,0x%08x,0x%08x,0x%04x,0x%04x,0x%04x,0x%04x,0x%04x,0x%04x,0x%08x,0x%02x,0x%02x,0x%04x",
                    f, powcnt, dispcnt, dispstat, vcount, keyin, keycnt, pc9, pc7, head, player, ctrl, gz0, gz1, gz2, p48, p4a, p4c,
                    velx, vely, grav, term, yvel, flags, zone, zone1c, zone1d, zmod);
            fprintf(fp, ",0x%08x,0x%08x,0x%08x,0x%08x,0x%08x,0x%08x,0x%08x,0x%04x,0x%08x,0x%08x,0x%08x,0x%04x,0x%04x,0x%08x,0x%08x,0x%08x,0x%02x,0x%08x,0x%08x,0x%08x,0x%08x,0x%08x,0x%04x\n",
                    touch0, touch4, touch8, touchc, touchflags, touch14,
                    taskcb, taskidx, tasktable, taskentry0, taskentry1, attract, topmode, z20, prevkey, modep, mode, mstate, ovact, ovreq, ovload, ovcb0, zmode);
        }
        // --dump-frame: dump memory at specific frames (continues running)
        for (size_t d = 0; d < g_dumpFrames.size(); d++) {
            if (f == g_dumpFrames[d].frame) {
                const DumpFrameRegion& r = g_dumpFrames[d];
                FILE* of = fopen(r.out, "wb");
                if (!of) { fprintf(stderr, "cannot write dump %s\n", r.out); continue; }
                for (u32 off = 0; off < r.len; off++) {
                    u8 b = _MMU_read08<ARMCPU_ARM9>(r.start + off);
                    fwrite(&b, 1, 1, of);
                }
                fclose(of);
                fprintf(stderr, "frame %d: dumped 0x%08x..0x%08x (%u B) -> %s\n",
                        f, r.start, r.start + r.len, r.len, r.out);
            }
        }
    }

    fclose(fp);

    for (size_t d = 0; d < g_dumps.size(); d++) {
        FILE* of = fopen(g_dumps[d].out, "wb");
        if (!of) { fprintf(stderr, "cannot write dump %s\n", g_dumps[d].out); continue; }
        u32 written = 0;
        for (u32 off = 0; off < g_dumps[d].len; off++) {
            u8 b = _MMU_read08<ARMCPU_ARM9>(g_dumps[d].start + off);
            fwrite(&b, 1, 1, of);
            written++;
        }
        fclose(of);
        fprintf(stderr, "dumped 0x%08x..0x%08x (%u B) -> %s\n",
                g_dumps[d].start, g_dumps[d].start + written, written, g_dumps[d].out);
    }

    NDS_DeInit();
    return 0;
}
