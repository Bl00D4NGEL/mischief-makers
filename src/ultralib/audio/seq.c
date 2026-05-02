#include <PR/libaudio.h>
#include <PR/os_internal.h>
#include <PR/ultraerror.h>
#include "audio/seq.h"

#define IFF_FILE_HDR 0x4D546864  // 'MThd'
#define IFF_TRACK_HDR 0x4D54726B // 'MTrk'

static s32 readVarLen(ALSeq* seq);
static u8 read8(ALSeq* seq);
static s16 read16(ALSeq* seq);
static s32 read32(ALSeq* seq);

void alSeqNew(ALSeq* seq, u8* ptr, s32 len) {
    seq->base = ptr;
    seq->len = len;
    seq->lastStatus = 0;
    seq->lastTicks = 0;
    seq->curPtr = ptr;

    if (read32(seq) != IFF_FILE_HDR) {
        __osError(ERR_ALSEQNOTMIDI, 1, ptr);
        return;
    }

    read32(seq);

    if (read16(seq) != 0) {
        __osError(ERR_ALSEQNOTMIDI0, 1, ptr);
        return;
    }

    if (read16(seq) != 1) {
        __osError(ERR_ALSEQNUMTRACKS, 1, ptr);
        return;
    }

    seq->division = read16(seq);
    if (seq->division & 0x8000) {
        __osError(ERR_ALSEQTIME, 1, ptr);
        return;
    }

    seq->qnpt = 1.0 / (f32)seq->division;

    if (read32(seq) != IFF_TRACK_HDR) {
        __osError(ERR_ALSEQTRACKHDR, 1, ptr);
        return;
    }

    read32(seq);
    seq->trackStart = seq->curPtr;
}

void alSeqNextEvent(ALSeq* seq, ALEvent* event) {
    u8 status;
    s16 delta;
    s32 len;
    s32 deltaTicks;
    s32 i;

    if (seq->curPtr >= seq->base + seq->len) {
        __osError(ERR_ALSEQOVERRUN, 0);
    }

    deltaTicks = readVarLen(seq);
    seq->lastTicks += deltaTicks;
    status = read8(seq);

    if ((status == 0xF0) || (status == 0xF7)) {
        __osError(ERR_ALSEQSYSEX, 0);
        len = readVarLen(seq);
        for (i = 0; i < len; i++) {
            read8(seq);
        }
        alSeqNextEvent(seq, event);
        return;
    }

    if (status == AL_MIDI_Meta) {
        u8 type = read8(seq);

        if (type == AL_MIDI_META_TEMPO) {
            event->type = AL_TEMPO_EVT;
            event->msg.tempo.ticks = deltaTicks;
            event->msg.tempo.status = status;
            event->msg.tempo.type = type;
            event->msg.tempo.len = read8(seq);
            event->msg.tempo.byte1 = read8(seq);
            event->msg.tempo.byte2 = read8(seq);
            event->msg.tempo.byte3 = read8(seq);
        }
        else if (type == AL_MIDI_META_EOT) {
            event->type = AL_SEQ_END_EVT;
            event->msg.end.ticks = deltaTicks;
            event->msg.end.status = status;
            event->msg.end.type = type;
            event->msg.end.len = read8(seq);
        }
        else {
            __osError(ERR_ALSEQMETA, 1, type);
            len = readVarLen(seq);
            for (i = 0; i < len; i++) {
                read8(seq);
            }
            alSeqNextEvent(seq, event);
            return;
        }

        seq->lastStatus = 0;
    }
    else {
        event->type = AL_SEQ_MIDI_EVT;
        event->msg.midi.ticks = deltaTicks;

        if (status & 0x80) {
            event->msg.midi.status = status;
            event->msg.midi.byte1 = read8(seq);
            seq->lastStatus = status;
        }
        else {
            event->msg.midi.status = seq->lastStatus;
            event->msg.midi.byte1 = status;
        }

        if (((event->msg.midi.status & 0xF0) != AL_MIDI_ProgramChange) &&
            ((event->msg.midi.status & 0xF0) != AL_MIDI_ChannelPressure)) {
            event->msg.midi.byte2 = read8(seq);
        }
        else {
            event->msg.midi.byte2 = 0;
        }
    }
}

char __alSeqNextDelta(ALSeq* seq, s32* pDeltaTicks) {
    u8* savedPtr;

    if (seq->curPtr >= seq->base + seq->len) {
        return FALSE;
    }

    savedPtr = seq->curPtr;
    *pDeltaTicks = readVarLen(seq);
    seq->curPtr = savedPtr;

    return TRUE;
}

f32 alSeqTicksToSec(ALSeq* seq, s32 ticks, u32 tempo) {
    return (f32)(((f32)ticks * (f32)tempo) / ((f32)seq->division * 1000000.0));
}

u32 alSeqSecToTicks(ALSeq* seq, f32 sec, u32 tempo) {
    return (u32)(((sec * 1000000.0) * seq->division) / tempo);
}

void alSeqNewMarker(ALSeq* seq, ALSeqMarker* marker, u32 ticks) {
    ALEvent event;
    u8* savePtr;
    u8* lastPtr;
    s32 saveTicks;
    s32 lastTicks;
    s16 saveStatus;
    s16 lastStatus;

    if (ticks == 0) {
        marker->curPtr = seq->trackStart;
        marker->lastStatus = 0;
        marker->lastTicks = 0;
        marker->curTicks = 0;
        return;
    }
    else {
        savePtr = seq->curPtr;
        saveStatus = seq->lastStatus;
        saveTicks = seq->lastTicks;

        seq->curPtr = seq->trackStart;
        seq->lastStatus = 0;
        seq->lastTicks = 0;

        do {
            lastPtr = seq->curPtr;
            lastStatus = seq->lastStatus;
            lastTicks = seq->lastTicks;

            alSeqNextEvent(seq, &event);

            if (event.type == AL_SEQ_END_EVT) {
                lastPtr = seq->curPtr;
                lastStatus = seq->lastStatus;
                lastTicks = seq->lastTicks;
                break;
            }
        } while (seq->lastTicks < ticks);

        marker->curPtr = lastPtr;
        marker->lastStatus = lastStatus;
        marker->lastTicks = lastTicks;
        marker->curTicks = seq->lastTicks;

        seq->curPtr = savePtr;
        seq->lastStatus = saveStatus;
        seq->lastTicks = saveTicks;
    }
}

s32 alSeqGetTicks(ALSeq* seq) {
    return seq->lastTicks;
}

void alSeqSetLoc(ALSeq* seq, ALSeqMarker* marker) {
    seq->curPtr = marker->curPtr;
    seq->lastStatus = marker->lastStatus;
    seq->lastTicks = marker->lastTicks;
}

void alSeqGetLoc(ALSeq* seq, ALSeqMarker* marker) {
    marker->curPtr = seq->curPtr;
    marker->lastStatus = seq->lastStatus;
    marker->lastTicks = seq->lastTicks;
}

static u8 read8(ALSeq* seq) {
    return *seq->curPtr++;
}

static s16 read16(ALSeq* seq) {
    s16 value;

    value = *seq->curPtr++ << 8;
    value |= *seq->curPtr++;

    return value;
}

static s32 read32(ALSeq* seq) {
    s32 value;

    value = *seq->curPtr++ << 24;
    value |= *seq->curPtr++ << 16;
    value |= *seq->curPtr++ << 8;
    value |= *seq->curPtr++;

    return value;
}

static s32 readVarLen(ALSeq* seq) {
    s32 value;
    s32 byte;

    byte = *seq->curPtr++;
    value = byte;

    if (byte & 0x80) {
        value &= 0x7F;
        do {
            byte = *seq->curPtr++;
            value = (value << 7) + (byte & 0x7F);
        } while (byte & 0x80);
    }

    return value;
}
