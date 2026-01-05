#pragma once
#include "PatchBase.h"

class PatchManager {
private:
    std::vector<std::unique_ptr<PatchBase>> patches;
    HANDLE hProcess;
    uintptr_t baseAddress;
    std::string iniPath;
    std::mutex patchMutex;
    bool patchesApplied = false;

public:
    PatchManager(HANDLE process, uintptr_t base, const std::string& ini)
        : hProcess(process), baseAddress(base), iniPath(ini) {
    }

    PatchManager(const PatchManager&) = delete;
    PatchManager& operator=(const PatchManager&) = delete;

    template<typename T>
    void RegisterPatch() {
        std::lock_guard<std::mutex> lock(patchMutex);
        patches.push_back(std::make_unique<T>(hProcess, baseAddress, iniPath));
    }

    void ApplyAllPatches() {
        std::lock_guard<std::mutex> lock(patchMutex);

        if (patchesApplied) {
            return;
        }

        for (auto& patch : patches) {
            if (patch->ShouldApply()) {
                patch->ApplySafe();
            }
        }

        patchesApplied = true;
    }
};