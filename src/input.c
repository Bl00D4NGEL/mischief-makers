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
