#include "common.h"
#include "boot.h"
#include "input.h"

void Input_Update(void);
u32 Input_GetFirstController(void);

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

u32 Input_GetFirstController(void) {
    s32 index;
    u8 controller_bits;

    osCreateMesgQueue(&gContInitMesgQ, &D_8012AC7C, 1);
    osSetEventMesg(OS_EVENT_SI, &gContInitMesgQ, 1);
    osContInit(&gContInitMesgQ, &controller_bits, gContStatus);

    osCreateMesgQueue(&D_8012AC08, &D_8012AC78, 1);
    osSetEventMesg(OS_EVENT_SI, &D_8012AC08, NULL);

    osCreateMesgQueue(&gContMesgQ, &D_8012ADB8, 2);
    osSetEventMesg(OS_EVENT_SI, &gContMesgQ, 2);

    for (index = 0; index < 4; index++) {
        if (controller_bits & (1 << index)) {
            if (!(gContStatus[index].errno & CONT_NO_RESPONSE_ERROR)) {
                return index;
            }
        }
    }

    return -1;
}

