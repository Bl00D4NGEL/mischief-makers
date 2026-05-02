#include <PR/libaudio.h>
#include <PR/os_internal.h>
#include <PR/ultraerror.h>

void _bnkfPatchBank(ALBank* bank, s32 offset, s32 table);
void _bnkfPatchInst(ALInstrument* inst, s32 offset, s32 table);
void _bnkfPatchSound(ALSound* sound, s32 offset, s32 table);
void _bnkfPatchWaveTable(ALWaveTable* wavetable, s32 offset, s32 table);

void alSeqFileNew(ALSeqFile* file, u8* base) {
    s32 offset = (s32)base;
    s32 index;

    // Patch the file so that offsets are pointers
    for (index = 0; index < file->seqCount; index++) {
        file->seqArray[index].offset = (u8*)((u8*)file->seqArray[index].offset + offset);
    }
}

void alBnkfNew(ALBankFile* file, u8* table) {
    s32 offset = (s32)file;
    s32 table_offset = (s32)table;
    s32 index;

    ALFailIf(file->revision != AL_BANK_VERSION, ERR_ALBNKFNEW);

    // Patch the file so that offsets are pointers.
    for (index = 0; index < file->bankCount; index++) {
        file->bankArray[index] = (ALBank*)((u8*)file->bankArray[index] + offset);
        if (file->bankArray[index]) {
            _bnkfPatchBank(file->bankArray[index], offset, table_offset);
        }
    }
}

void _bnkfPatchBank(ALBank* bank, s32 offset, s32 table) {
    s32 index;

    if (bank->flags) {
        return;
    }

    bank->flags = 1;

    if (bank->percussion) {
        bank->percussion = (ALInstrument*)((u8*)bank->percussion + offset);
        _bnkfPatchInst(bank->percussion, offset, table);
    }

    for (index = 0; index < bank->instCount; index++) {
        bank->instArray[index] = (ALInstrument*)((u8*)bank->instArray[index] + offset);
        if (bank->instArray[index]) {
            _bnkfPatchInst(bank->instArray[index], offset, table);
        }
    }
}

void _bnkfPatchInst(ALInstrument* inst, s32 offset, s32 table) {
    s32 index;

    if (inst->flags) {
        return;
    }

    inst->flags = 1;

    for (index = 0; index < inst->soundCount; index++) {
        inst->soundArray[index] = (ALSound*)((u8*)inst->soundArray[index] + offset);
        _bnkfPatchSound(inst->soundArray[index], offset, table);
    }
}

void _bnkfPatchSound(ALSound* sound, s32 offset, s32 table) {
    if (sound->flags) {
        return;
    }

    sound->flags = 1;

    sound->envelope = (ALEnvelope*)((u8*)sound->envelope + offset);
    sound->keyMap = (ALKeyMap*)((u8*)sound->keyMap + offset);
    sound->wavetable = (ALWaveTable*)((u8*)sound->wavetable + offset);
    _bnkfPatchWaveTable(sound->wavetable, offset, table);
}

void _bnkfPatchWaveTable(ALWaveTable* wavetable, s32 offset, s32 table) {
    if (wavetable->flags) {
        return;
    }

    wavetable->flags = 1;
    wavetable->base += table;

    if (wavetable->type == AL_ADPCM_WAVE) {
        wavetable->waveInfo.adpcmWave.book = (ALADPCMBook*)((u8*)wavetable->waveInfo.adpcmWave.book + offset);

        if (wavetable->waveInfo.adpcmWave.loop) {
            wavetable->waveInfo.adpcmWave.loop = (ALADPCMloop*)((u8*)wavetable->waveInfo.adpcmWave.loop + offset);
        }
    }
    else if (wavetable->type == AL_RAW16_WAVE) {
        if (wavetable->waveInfo.rawWave.loop) {
            wavetable->waveInfo.rawWave.loop = (ALRawLoop*)((u8*)wavetable->waveInfo.rawWave.loop + offset);
        }
    }
}
