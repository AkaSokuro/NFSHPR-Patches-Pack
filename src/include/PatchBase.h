#pragma once

class PatchBase {
protected:
    HANDLE hProcess;
    uintptr_t baseAddress;
    std::string iniPath;
    std::atomic<bool> applied{ false };

    static std::mutex memoryWriteMutex;

    bool WriteMemory(uintptr_t address, const unsigned char* data, size_t size) {
        std::lock_guard<std::mutex> lock(memoryWriteMutex);
        return WriteProcessMemory(hProcess, (LPVOID)address, data, size, NULL) != 0;
    }

public:
    PatchBase(HANDLE process, uintptr_t base, const std::string& ini)
        : hProcess(process), baseAddress(base), iniPath(ini) {
    }

    virtual ~PatchBase() = default;

    PatchBase(const PatchBase&) = delete;
    PatchBase& operator=(const PatchBase&) = delete;

    virtual const char* GetName() const = 0;
    virtual const char* GetIniSection() const = 0;
    virtual const char* GetIniKey() const = 0;
    virtual int GetDefaultEnabled() const = 0;

    virtual bool ShouldApply() {
        return GetPrivateProfileIntA(GetIniSection(), GetIniKey(), GetDefaultEnabled(), iniPath.c_str()) != 0;
    }

    bool IsApplied() const {
        return applied.load();
    }

    void ApplySafe() {
        bool expected = false;
        if (applied.compare_exchange_strong(expected, true)) {
            Apply();
        }
    }

    virtual void Apply() = 0;
};

inline std::mutex PatchBase::memoryWriteMutex;