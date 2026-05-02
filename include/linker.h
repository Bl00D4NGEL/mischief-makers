#ifndef LINKER_H
#define LINKER_H

#include <PR/ultratypes.h>

extern u8 _bssSegmentStart[];
extern u8 _bssSegmentEnd[];
extern u8 _bssSegmentSize[];
extern u64 __gBootStackEnd[];
extern u64 __gIdleStackEnd[];
extern u64 __gMainStackEnd[];
extern u64 __gRmonStackEnd[];

#endif
