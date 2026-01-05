#pragma once
#include "../include/PatchBase.h"

class GamePriorityPatch : public PatchBase {
public:
    using PatchBase::PatchBase;

    const char* GetName() const override { return "GamePriority"; }
    const char* GetIniSection() const override { return "PATCHES"; }
    const char* GetIniKey() const override { return "iGamePriority"; }
    int GetDefaultEnabled() const override { return 3; }

    bool ShouldApply() override {
        return true;
    }

    void Apply() override {
        int gamePriority = GetPrivateProfileIntA(GetIniSection(), GetIniKey(), GetDefaultEnabled(), iniPath.c_str());

        DWORD priorityClass = NORMAL_PRIORITY_CLASS;
        switch (gamePriority) {
        case 1:
            priorityClass = IDLE_PRIORITY_CLASS;
            break;
        case 2:
            priorityClass = BELOW_NORMAL_PRIORITY_CLASS;
            break;
        case 3:
            priorityClass = NORMAL_PRIORITY_CLASS;
            break;
        case 4:
            priorityClass = ABOVE_NORMAL_PRIORITY_CLASS;
            break;
        case 5:
            priorityClass = HIGH_PRIORITY_CLASS;
            break;
        case 6:
            priorityClass = REALTIME_PRIORITY_CLASS;
            break;
        default:
            break;
        }
        SetPriorityClass(hProcess, priorityClass);
    }
};