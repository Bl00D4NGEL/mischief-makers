#include "common.h"
#include "thread.h"
#include "input.h"

// forward declarations

void func_80000450(void);
void boot(s32 unused);
void Thread_IdleProc(void* arg0);
void func_800008E0(void);
void Thread_MainProc(void* arg0);

// data

extern Gfx D_800E38B0[];
extern Gfx D_800E3930[];

extern OSMesgQueue sDMAMesgQ;
extern OSMesgQueue D_8012ABC0;
extern OSMesgQueue D_8012ABD8;
extern OSMesgQueue D_8012ABF0;
extern OSMesgQueue D_8012AC08; // might be in input.c

extern OSMesg D_8012AC68;
extern OSMesg D_8012AC6C;
extern OSMesg D_8012AC70;
extern OSMesg D_8012AC74;
extern OSMesg D_8012AC80;

// text
// needs loop rerolling
#ifdef NON_MATCHING
typedef struct {
    /* 0x000 */ u32 unk_0x00;
    /* 0x004 */ u32 unk_0x04;
    /* 0x008 */ u32 unk_0x08;
    /* 0x00C */ u32 unk_0x0C;
    /* 0x010 */ u32 unk_0x10;
    /* 0x014 */ u32 unk_0x14;
    /* 0x018 */ u32 unk_0x18;
    /* 0x01C */ u32 unk_0x1C;
    /* 0x020 */ u32 unk_0x20;
    /* 0x024 */ u32 unk_0x24;
    /* 0x028 */ u32 unk_0x28;
    /* 0x02C */ u32 unk_0x2C;
    /* 0x030 */ u32 unk_0x30;
    /* 0x034 */ u32 unk_0x34;
    /* 0x038 */ u32 unk_0x38;
    /* 0x03C */ u32 unk_0x3C;
    /* 0x040 */ u32 unk_0x40;
    /* 0x044 */ u32 unk_0x44;
    /* 0x048 */ u32 unk_0x48;
    /* 0x04C */ u32 unk_0x4C;
    /* 0x050 */ u32 unk_0x50;
    /* 0x054 */ u32 unk_0x54;
    /* 0x058 */ u32 unk_0x58;
    /* 0x05C */ u32 unk_0x5C;
    /* 0x060 */ u32 unk_0x60;
    /* 0x064 */ u32 unk_0x64;
    /* 0x068 */ u32 unk_0x68;
    /* 0x06C */ u32 unk_0x6C;
    /* 0x070 */ u32 unk_0x70;
    /* 0x074 */ u32 unk_0x74;
    /* 0x078 */ u32 unk_0x78;
    /* 0x07C */ u32 unk_0x7C;
    /* 0x080 */ u32 unk_0x80;
    /* 0x084 */ u32 unk_0x84;
    /* 0x088 */ u32 unk_0x88;
    /* 0x08C */ u32 unk_0x8C;
    /* 0x090 */ u32 unk_0x90;
    /* 0x094 */ u32 unk_0x94;
    /* 0x098 */ u32 unk_0x98;
    /* 0x09C */ u32 unk_0x9C;
    /* 0x0A0 */ u32 unk_0xA0;
    /* 0x0A4 */ u32 unk_0xA4;
    /* 0x0A8 */ u32 unk_0xA8;
    /* 0x0AC */ u32 unk_0xAC;
    /* 0x0B0 */ u32 unk_0xB0;
    /* 0x0B4 */ u32 unk_0xB4;
    /* 0x0B8 */ u32 unk_0xB8;
    /* 0x0BC */ u32 unk_0xBC;
    /* 0x0C0 */ u32 unk_0xC0;
    /* 0x0C4 */ u32 unk_0xC4;
    /* 0x0C8 */ u32 unk_0xC8;
    /* 0x0CC */ u32 unk_0xCC;
    /* 0x0D0 */ u32 unk_0xD0;
    /* 0x0D4 */ u32 unk_0xD4;
    /* 0x0D8 */ u32 unk_0xD8;
    /* 0x0DC */ u32 unk_0xDC;
    /* 0x0E0 */ u32 unk_0xE0;
    /* 0x0E4 */ u32 unk_0xE4;
    /* 0x0E8 */ u32 unk_0xE8;
    /* 0x0EC */ u32 unk_0xEC;
    /* 0x0F0 */ u32 unk_0xF0;
    /* 0x0F4 */ u32 unk_0xF4;
    /* 0x0F8 */ u32 unk_0xF8;
    /* 0x0FC */ u32 unk_0xFC;
    /* 0x100 */ u32 unk_0x100;
    /* 0x104 */ u32 unk_0x104;
    /* 0x108 */ u32 unk_0x108;
    /* 0x10C */ u32 unk_0x10C;
    /* 0x110 */ u32 unk_0x110;
    /* 0x114 */ u32 unk_0x114;
    /* 0x118 */ u32 unk_0x118;
    /* 0x11C */ u32 unk_0x11C;
    /* 0x120 */ u32 unk_0x120;
    /* 0x124 */ u32 unk_0x124;
    /* 0x128 */ u32 unk_0x128;
    /* 0x12C */ u32 unk_0x12C;
    /* 0x130 */ u32 unk_0x130;
    /* 0x134 */ u32 unk_0x134;
    /* 0x138 */ u32 unk_0x138;
    /* 0x13C */ u32 unk_0x13C;
    /* 0x140 */ u32 unk_0x140;
    /* 0x144 */ u32 unk_0x144;
    /* 0x148 */ u32 unk_0x148;
    /* 0x14C */ u32 unk_0x14C;
    /* 0x150 */ u32 unk_0x150;
    /* 0x154 */ u32 unk_0x154;
    /* 0x158 */ u32 unk_0x158;
    /* 0x15C */ u32 unk_0x15C;
    /* 0x160 */ u32 unk_0x160;
    /* 0x164 */ u32 unk_0x164;
    /* 0x168 */ u32 unk_0x168;
    /* 0x16C */ u32 unk_0x16C;
    /* 0x170 */ u32 unk_0x170;
    /* 0x174 */ u32 unk_0x174;
    /* 0x178 */ u32 unk_0x178;
    /* 0x17C */ u32 unk_0x17C;
    /* 0x180 */ u32 unk_0x180;
    /* 0x184 */ u32 unk_0x184;
    /* 0x188 */ u32 unk_0x188;
    /* 0x18C */ u32 unk_0x18C;
    /* 0x190 */ u32 unk_0x190;
    /* 0x194 */ u32 unk_0x194;
    /* 0x198 */ u32 unk_0x198;
    /* 0x19C */ u32 unk_0x19C;
    /* 0x1A0 */ u32 unk_0x1A0;
    /* 0x1A4 */ u32 unk_0x1A4;
    /* 0x1A8 */ u32 unk_0x1A8;
    /* 0x1AC */ u32 unk_0x1AC;
    /* 0x1B0 */ u32 unk_0x1B0;
    /* 0x1B4 */ u32 unk_0x1B4;
    /* 0x1B8 */ u32 unk_0x1B8;
    /* 0x1BC */ u32 unk_0x1BC;
    /* 0x1C0 */ u32 unk_0x1C0;
    /* 0x1C4 */ u32 unk_0x1C4;
    /* 0x1C8 */ u32 unk_0x1C8;
    /* 0x1CC */ u32 unk_0x1CC;
    /* 0x1D0 */ u32 unk_0x1D0;
    /* 0x1D4 */ u32 unk_0x1D4;
    /* 0x1D8 */ u32 unk_0x1D8;
    /* 0x1DC */ u32 unk_0x1DC;
    /* 0x1E0 */ u32 unk_0x1E0;
    /* 0x1E4 */ u32 unk_0x1E4;
    /* 0x1E8 */ u32 unk_0x1E8;
    /* 0x1EC */ u32 unk_0x1EC;
    /* 0x1F0 */ u32 unk_0x1F0;
    /* 0x1F4 */ u32 unk_0x1F4;
    /* 0x1F8 */ u32 unk_0x1F8;
    /* 0x1FC */ u32 unk_0x1FC;
} unk_func_80000450; /* sizeof = 0x200 */

void func_80000450(void) {
    s32 var_v1; // register v1
    unk_func_80000450* var_v0; // register v0
    u32 t0; // register t0
    u32 t2; // register t2
    u32 t4; // register t4
    u32 t6; // register t6
    u32 t7; // register t7
    u32 t8; // register t8

    osViBlack(1);

    var_v0 = 0x803DA800; // framebuffer

    // 0x4B0 / 4 = 300
    // (0x4B0 / 4) * 0x200 = 0x25800
    // 320 x 240 * 2 = 0x25800
    for (var_v1 = 0; var_v1 != 0x4B0; var_v1 += 4) {
        t7 = 0x10001; // used on non-8-byte-aligned offsets
        t0 = 0; // these are li'd
        t2 = 0; // these are li'd
        t4 = 0; // these are li'd
        t6 = 0; // these are li'd
        t8 = 0; // these are li'd

        // 9
        var_v0->unk_0xE0 = t4; // all groups uses -0x28
        var_v0->unk_0xB8 = t4;
        var_v0->unk_0x90 = t4;

        // and are + 0x88 apart
        var_v0->unk_0x168 = t4;
        var_v0->unk_0x140 = t4;
        var_v0->unk_0x118 = t4;

        var_v0->unk_0x1F0 = t4;
        var_v0->unk_0x1C8 = t4;
        var_v0->unk_0x1A0 = t4;

        // 10
        var_v0->unk_0xE8 = t2;
        var_v0->unk_0xC0 = t2;
        var_v0->unk_0x98 = t2;

        var_v0->unk_0x170 = t2;
        var_v0->unk_0x148 = t2;
        var_v0->unk_0x120 = t2;

        var_v0->unk_0x1F8 = t2;
        var_v0->unk_0x1D0 = t2;
        var_v0->unk_0x1A8 = t2;
        var_v0->unk_0x180 = t2;

        // 10
        var_v0->unk_0xF0 = t0;
        var_v0->unk_0xC8 = t0;
        var_v0->unk_0xA0 = t0;

        var_v0->unk_0x178 = t0;
        var_v0->unk_0x150 = t0;
        var_v0->unk_0x128 = t0;
        var_v0->unk_0x100 = t0;

        var_v0->unk_0x1D8 = t0;
        var_v0->unk_0x1B0 = t0;
        var_v0->unk_0x188 = t0;

        // 10
        var_v0->unk_0xF8 = t8;
        var_v0->unk_0xD0 = t8;
        var_v0->unk_0xA8 = t8;
        var_v0->unk_0x80 = t8;

        var_v0->unk_0x158 = t8;
        var_v0->unk_0x130 = t8;
        var_v0->unk_0x108 = t8;

        var_v0->unk_0x1E0 = t8;
        var_v0->unk_0x1B8 = t8;
        var_v0->unk_0x190 = t8;

        // 9
        var_v0->unk_0xD8 = t6;
        var_v0->unk_0xB0 = t6;
        var_v0->unk_0x88 = t6;

        var_v0->unk_0x160 = t6;
        var_v0->unk_0x138 = t6;
        var_v0->unk_0x110 = t6;

        var_v0->unk_0x1E8 = t6;
        var_v0->unk_0x1C0 = t6;
        var_v0->unk_0x198 = t6;

        // 9 + 10 + 10 + 10 + 9 = 48
        var_v0->unk_0xFC = t7;
        var_v0->unk_0xF4 = t7;

        var_v0->unk_0xEC = t7;
        var_v0->unk_0xE4 = t7;

        var_v0->unk_0xDC = t7;
        var_v0->unk_0xD4 = t7;

        var_v0->unk_0xCC = t7;
        var_v0->unk_0xC4 = t7;

        var_v0->unk_0xBC = t7;
        var_v0->unk_0xB4 = t7;

        var_v0->unk_0xAC = t7;
        var_v0->unk_0xA4 = t7;

        var_v0->unk_0x9C = t7;
        var_v0->unk_0x94 = t7;

        var_v0->unk_0x8C = t7;
        var_v0->unk_0x84 = t7;

        var_v0->unk_0x17C = t7;
        var_v0->unk_0x174 = t7;

        var_v0->unk_0x16C = t7;
        var_v0->unk_0x164 = t7;

        var_v0->unk_0x15C = t7;
        var_v0->unk_0x154 = t7;

        var_v0->unk_0x14C = t7;
        var_v0->unk_0x144 = t7;

        var_v0->unk_0x13C = t7;
        var_v0->unk_0x134 = t7;

        var_v0->unk_0x12C = t7;
        var_v0->unk_0x124 = t7;

        var_v0->unk_0x11C = t7;
        var_v0->unk_0x114 = t7;

        var_v0->unk_0x10C = t7;
        var_v0->unk_0x104 = t7;

        var_v0->unk_0x1FC = t7;
        var_v0->unk_0x1F4 = t7;

        var_v0->unk_0x1EC = t7;
        var_v0->unk_0x1E4 = t7;

        var_v0->unk_0x1DC = t7;
        var_v0->unk_0x1D4 = t7;

        var_v0->unk_0x1CC = t7;
        var_v0->unk_0x1C4 = t7;

        var_v0->unk_0x1BC = t7;
        var_v0->unk_0x1B4 = t7;

        var_v0->unk_0x1AC = t7;
        var_v0->unk_0x1A4 = t7;

        var_v0->unk_0x19C = t7;
        var_v0->unk_0x194 = t7;

        var_v0->unk_0x18C = t7;
        var_v0->unk_0x184 = t7;

        // + 0x200?
        var_v0++;

        // 3
        var_v0[-1].unk_0x58 = t4; // -0x1A8
        var_v0[-1].unk_0x30 = t4; // -0x1D0
        var_v0[-1].unk_0x08 = t4; // -0x1F8

        // 3
        var_v0[-1].unk_0x60 = t2; // -0x1A0
        var_v0[-1].unk_0x38 = t2; // -0x1C8
        var_v0[-1].unk_0x10 = t2; // -0x1F0

        // 3
        var_v0[-1].unk_0x68 = t0; // -0x198
        var_v0[-1].unk_0x40 = t0; // -0x1C0
        var_v0[-1].unk_0x18 = t0; // -0x1E8

        // 3
        var_v0[-1].unk_0x70 = t8; // -0x190
        var_v0[-1].unk_0x48 = t8; // -0x1B8
        var_v0[-1].unk_0x20 = t8; // -0x1E0

        // 4
        var_v0[-1].unk_0x78 = t6; // -0x188
        var_v0[-1].unk_0x50 = t6; // -0x1B0
        var_v0[-1].unk_0x28 = t6; // -0x1D8
        var_v0[-1].unk_0x00 = t6; // -0x200

        // 3 + 3 + 3 + 3 + 4 = 16
        var_v0[-1].unk_0x7C = t7; // -0x184
        var_v0[-1].unk_0x74 = t7; // -0x18C
        var_v0[-1].unk_0x6C = t7; // -0x194
        var_v0[-1].unk_0x64 = t7; // -0x19C
        var_v0[-1].unk_0x5C = t7; // -0x1A4
        var_v0[-1].unk_0x54 = t7; // -0x1AC
        var_v0[-1].unk_0x4C = t7; // -0x1B4
        var_v0[-1].unk_0x44 = t7; // -0x1BC
        var_v0[-1].unk_0x3C = t7; // -0x1C4
        var_v0[-1].unk_0x34 = t7; // -0x1CC
        var_v0[-1].unk_0x2C = t7; // -0x1D4
        var_v0[-1].unk_0x24 = t7; // -0x1DC
        var_v0[-1].unk_0x1C = t7; // -0x1E4
        var_v0[-1].unk_0x14 = t7; // -0x1EC
        var_v0[-1].unk_0x0C = t7; // -0x1F4
        var_v0[-1].unk_0x04 = t7; // -0x1FC
    }

    osViSwapBuffer(0x803DA800);
    osViBlack(0);
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/boot/func_80000450.s")
#endif

void boot(s32 unused) {
    osInitialize();
    osCreateThread(&sIdleThread, 1, Thread_IdleProc, NULLPTR, __gIdleStackEnd, 10);
    osStartThread(&sIdleThread);
}

void Thread_IdleProc(void* arg0) {
    osCreateViManager(OS_PRIORITY_VIMGR);
    if (osTvType == OS_TV_MPAL) {
        osViSetMode(&osViModeTable[OS_VI_MPAL_LAN1]);
        func_80000450();
        gOSViMode = osViModeTable[OS_VI_MPAL_LAN1];
        gOSViModep = &osViModeTable[OS_VI_MPAL_LAN1];
    }
    else {
        osViSetMode(&osViModeTable[OS_VI_NTSC_LAN1]);
        func_80000450();
        gOSViMode = osViModeTable[OS_VI_NTSC_LAN1];
        gOSViModep = &osViModeTable[OS_VI_NTSC_LAN1];
    }

    osCreatePiManager(OS_PRIORITY_PIMGR, &D_8012AC38, &D_8012A678, 8);
    osCreateThread(&sRmonThread, 0, rmonMain, NULL, __gRmonStackEnd, OS_PRIORITY_RMON);
    osStartThread(&sRmonThread);
    osCreateThread(&sMainThread, 3, Thread_MainProc, arg0, __gMainStackEnd, 10);
    osStartThread(&sMainThread);
    osSetThreadPri(0, 0);

    while (1) {}
}

void func_800008E0(void) {
    u32 vStart0;
    u32 vStart1;
    
    // TODO: cleanup fakematch

    vStart0 = gOSViMode.comRegs.hStart;
    gOSViModep->comRegs.hStart = ((((((((s32)vStart0) >> 0x10) & 0xFFFF) + 0xA)) % 0xFFFF) << 0x10)
                            | (((s32) ((vStart0 & 0xFFFF) + 0xA)) % 0xFFFF);
    if (1) {
        vStart0 = gOSViMode.fldRegs[0].vStart;
        gOSViModep->fldRegs[0].vStart = ((((((((s32) gOSViMode.fldRegs[0].vStart) >> 0x10) & 0xFFFF) + 8)) % 0xFFFF) << 0x10)
                                    | (((s32) ((vStart0 & 0xFFFF) + 8)) % 0xFFFF);
    }
    vStart1 = gOSViMode.fldRegs[1].vStart;
    gOSViModep->fldRegs[1].vStart = ((((((((s32) vStart1) >> 0x10) & 0xFFFF) + 8)) % 0xFFFF) << 0x10)
                                | (((s32) ((vStart1 & 0xFFFF) + 8)) % 0xFFFF);
}

// main update setup
void func_80000A84(u16 buffer_index) {
    u16* framebuffer;

    gGFXTaskp = &gGFXTasks[buffer_index];
    D_800EF4F4 = gDListTail[buffer_index].unk_0x00;
    gDListHead = D_800EF4F4 + 48;

    if (buffer_index) {
        framebuffer = FRAMEBUFFER0;
    }
    else {
        framebuffer = FRAMEBUFFER1;
    }

    gSPSegment(gDListHead++, 0, 0);
    gSPSegment(gDListHead++, 6, osVirtualToPhysical(D_800EF4F4));
    gSPDisplayList(gDListHead++, D_800E3930);
    gSPDisplayList(gDListHead++, D_800E38B0);
    gDPPipeSync(gDListHead++);
    gDPSetColorImage(gDListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 320, osVirtualToPhysical(framebuffer));

    func_8000147C();

    gDPFullSync(gDListHead++);
    gSPEndDisplayList(gDListHead++);
}

void Thread_MainProc(void* arg0) {
    u16* framebuffer;

    Sound_InitPlayers();
    osCreateMesgQueue(&sDMAMesgQ, &D_8012AC68, 1);
    osCreateMesgQueue(&D_8012ABD8, &D_8012AC70, 1);
    osSetEventMesg(OS_EVENT_SP, &D_8012ABD8, D_8012AC80);
    Sound_SetEventMesg();
    osCreateMesgQueue(&D_8012ABF0, &D_8012AC74, 1);
    osSetEventMesg(OS_EVENT_DP, &D_8012ABF0, D_8012AC80);
    osCreateMesgQueue(&D_8012ABC0, &D_8012AC6C, 1);
    osViSetSpecialFeatures(OS_VI_GAMMA_OFF | OS_VI_GAMMA_DITHER_OFF);
    osViSetEvent(&D_8012ABC0, D_8012AC80, 1);

    func_800008E0();
    func_80022D10();
    func_80000A84(gCurrentFramebufferIndex);

    gPlayerControllerIndex = Input_GetFirstController();

    framebuffer = FRAMEBUFFER1;

    while (1) {
        osContStartReadData(&gContMesgQ);
        osRecvMesg(&gContMesgQ, NULL, 1);
        osContGetReadData(gContpadArrayB);
        if (gPlayerControllerIndex != -1) {
            osContStartReadData(&D_8012AC08);
        }

        gGFXTaskp->t.type = M_GFXTASK;
        gGFXTaskp->t.flags = 0;
        gGFXTaskp->t.ucode_boot = (u64*)rspbootTextStart;
        gGFXTaskp->t.ucode_boot_size = (u32)rspbootTextEnd - (u32)rspbootTextStart;
        gGFXTaskp->t.ucode = (u64*)gspFast3DTextStart;
        gGFXTaskp->t.ucode_data = (u64*)gspFast3DDataStart;
        gGFXTaskp->t.output_buff = NULL;
        gGFXTaskp->t.output_buff_size = NULL;
        gGFXTaskp->t.ucode_size = 0x1000;
        gGFXTaskp->t.ucode_data_size = 0x800;
        gGFXTaskp->t.dram_stack = gDramStack;
        gGFXTaskp->t.dram_stack_size = sizeof(gDramStack);
        gGFXTaskp->t.data_ptr = gDListTail[gCurrentFramebufferIndex].dlist;
        gGFXTaskp->t.data_size = (u32)((gDListHead - gDListTail[gCurrentFramebufferIndex].dlist) * sizeof(Gfx));
        gGFXTaskp->t.yield_data_ptr = gYeildData;
        gGFXTaskp->t.yield_data_size = sizeof(gYeildData);

        osWritebackDCacheAll();
        osSpTaskLoad(gGFXTaskp);
        osSpTaskStartGo(gGFXTaskp);
        Sound_Update();
        Sound_NextBuffer();

        gCurrentFramebufferIndex = 1 - gCurrentFramebufferIndex;

        func_80000A84(gCurrentFramebufferIndex);
        osRecvMesg(&D_8012ABF0, NULL, 1);
        Sound_StartTask();
        osViSwapBuffer(framebuffer);
        LookAt_Update();

        // D_8012ABC0 is probably the retrace/vsync queue
        if (MQ_IS_FULL(&D_8012ABC0)) {
            Sound_Update();
            Sound_NextBuffer();
            osRecvMesg(&D_8012ABC0, &D_8012AC80, 1);
            Sound_StartTask();
        }

        osRecvMesg(&D_8012ABC0, &D_8012AC80, 1);

        if (gCurrentFramebufferIndex) {
            framebuffer = FRAMEBUFFER0; // framebuffer = gFramebuffer0;
        }
        else {
            framebuffer = FRAMEBUFFER1; // framebuffer = gFramebuffer1;
        }

        Input_Update();
    }
    Sound_Update();
}

