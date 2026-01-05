#pragma once
#include "../include/PatchBase.h"

class MatchEndCrashFixPatch : public PatchBase {
public:
    using PatchBase::PatchBase;

    const char* GetName() const override { return "MatchEndCrashFix"; }
    const char* GetIniSection() const override { return "PATCHES"; }
    const char* GetIniKey() const override { return "bMatchEndCrashFix"; }
    int GetDefaultEnabled() const override { return 1; }

    void Apply() override {
        uintptr_t addr1 = baseAddress + 0x1A644E08;
        unsigned char patch1[] = {
            0x48, 0x85, 0xC9, 0x75, 0x0F, 0x48, 0x85, 0xD2,
            0x75, 0x0A, 0xF3, 0x0F, 0x59, 0x35, 0x96, 0x1A,
            0xC3, 0xE6, 0xEB, 0x06, 0xF3, 0x0F, 0x59, 0x74,
            0x11, 0x20, 0xE9, 0x35, 0xC4, 0xF8, 0xE5
        };

        uintptr_t addr2 = baseAddress + 0x5D1256;
        unsigned char patch2[] = {
            0xE9, 0xAD, 0x3B, 0x07, 0x1A, 0x90
        };

        WriteMemory(addr1, patch1, sizeof(patch1));
        WriteMemory(addr2, patch2, sizeof(patch2));
    }
};