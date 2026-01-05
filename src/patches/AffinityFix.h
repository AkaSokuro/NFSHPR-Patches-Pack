#pragma once
#include "../include/PatchBase.h"

class AffinityFixPatch : public PatchBase {
public:
    using PatchBase::PatchBase;

    const char* GetName() const override { return "AffinityFix"; }
    const char* GetIniSection() const override { return "PATCHES"; }
    const char* GetIniKey() const override { return "bAffinityFix"; }
    int GetDefaultEnabled() const override { return 1; }

    void Apply() override {
        DWORD_PTR systemAffinityMask, processAffinityMask;
        if (GetProcessAffinityMask(hProcess, &processAffinityMask, &systemAffinityMask)) {
            DWORD_PTR newAffinity = systemAffinityMask & ~1;
            if (newAffinity != 0) {
                SetProcessAffinityMask(hProcess, newAffinity);
            }
        }
    }
};