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

extern u8 audio_bank_ctl_ROM_START[];
extern u8 audio_bank_ctl_ROM_END[];
extern u8 audio_bank_tbl_ROM_START[];
extern u8 audio_seq_bank_ROM_START[];
extern u8 audio_seq_bank_DATA_START[];

#endif
