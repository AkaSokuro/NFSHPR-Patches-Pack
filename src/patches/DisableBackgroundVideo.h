#pragma once
#pragma once
#include "../include/PatchBase.h"

class DisableBackgroundVideoPatch : public PatchBase {
public:
    using PatchBase::PatchBase;

    const char* GetName() const override { return "DisableBackgroundVideo"; }
    const char* GetIniSection() const override { return "PATCHES"; }
    const char* GetIniKey() const override { return "bDisableBackgroundVideo"; }
    int GetDefaultEnabled() const override { return 0; }

    void Apply() override {
        uintptr_t addr1 = baseAddress + 0x1D274A;
        unsigned char patch1[] = {
            0xC7, 0x83, 0xA8, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00
        };

        WriteMemory(addr1, patch1, sizeof(patch1));
    }
};