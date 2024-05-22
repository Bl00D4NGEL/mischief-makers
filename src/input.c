#include "common.h"
#include "input.h"
#include "thread.h"

void Input_Update(void) {
    osContGetReadData(gContpadArrayB);

    if (!gContpadArrayA[gPlayerControllerIndex].errno) {
    }

    osContGetReadData(gContpadArrayA);

    if (gContpadArrayA[gPlayerControllerIndex].errno == 0) {
        gButtonCurrent = gContpadArrayA[gPlayerControllerIndex].button;
        gJoyX = gContpadArrayA[gPlayerControllerIndex].stick_x;
        gJoyY = gContpadArrayA[gPlayerControllerIndex].stick_y;
    }
    else {
        gButtonCurrent = 0;
    }

    gButtonPress = (gButtonCurrent ^ gButtonLast) & gButtonCurrent;
    gButtonHold = gButtonCurrent;
    gButtonLast = gButtonCurrent;
}

#ifdef NON_MATCHING
u32 Input_GetFirstController(void) {
    u8 sp23;
    s32 var_t2;
    s32 var_t5;
    s32 var_t9;

    osCreateMesgQueue(&D_8012AC20, &D_8012AC7C, 1);
    osSetEventMesg(5U, &D_8012AC20, (void* )1);
    osContInit(&D_8012AC20, &sp23, &D_8012AD60);
    osCreateMesgQueue(&D_8012AC08, &D_8012AC78, 1);
    osSetEventMesg(5U, &D_8012AC08, NULL);
    osCreateMesgQueue(&gContMesgQ, &D_8012ADB8, 2);
    osSetEventMesg(5U, &gContMesgQ, (void* )2);
    if (sp23 & 1) {
        var_t9 = sp23 & 2;
        if (!(D_8012AD63 & 8)) {
            return 0U;
        }
        goto block_4;
    }
    var_t9 = sp23 & 2;
block_4:
    if (var_t9 != 0) {
        var_t2 = sp23 & 4;
        if (!(D_8012AD67 & 8)) {
            return 1U;
        }
        goto block_8;
    }
    var_t2 = sp23 & 4;
block_8:
    if (var_t2 != 0) {
        var_t5 = sp23 & 8;
        if (!(D_8012AD6B & 8)) {
            return 2U;
        }
        goto block_12;
    }
    var_t5 = sp23 & 8;
block_12:
    if (var_t5 != 0) {
        if (!(D_8012AD6F & 8)) {
            return 3U;
        }
        /* Duplicate return node #16. Try simplifying control flow for better match */
        return -1U;
    }
    return -1U;
}
#else
#pragma GLOBAL_ASM("asm/nonmatchings/input/Input_GetFirstController.s")
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/input/func_800011F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/func_80001264.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/func_80001290.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/func_800012F0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/func_8000147C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/func_80001670.s")

#pragma GLOBAL_ASM("asm/nonmatchings/input/func_8000178C.s")
