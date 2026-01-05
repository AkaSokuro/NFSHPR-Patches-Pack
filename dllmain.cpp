#include "pch.h"
#include <Windows.h>
#include <Shlwapi.h>

#pragma comment(lib, "Shlwapi.lib")

#include "src/include/Ini.h"
#include "src/include/PatchManager.h"
#include "src/patches/AffinityFix.h"
#include "src/patches/GamePriority.h"
#include "src/patches/FriendlistPatch.h"
#include "src/patches/MatchEndCrashFix.h"

void InitializePatches(HMODULE hModule) {
    char dllPath[MAX_PATH];
    GetModuleFileNameA(hModule, dllPath, MAX_PATH);
    PathRemoveFileSpecA(dllPath);

    char iniPath[MAX_PATH];
    sprintf_s(iniPath, "%s\\patches.ini", dllPath);

    if (!Ini::EnsureIniExists(iniPath)) {
        return;
    }

    int patchEnabled = GetPrivateProfileIntA("CONFIGS", "PatchEnabled", 1, iniPath);
    if (!patchEnabled) {
        return;
    }

    HANDLE hProcess = GetCurrentProcess();
    uintptr_t baseAddress = (uintptr_t)GetModuleHandle(NULL);

    PatchManager manager(hProcess, baseAddress, iniPath);

    manager.RegisterPatch<AffinityFixPatch>();
    manager.RegisterPatch<GamePriorityPatch>();
    manager.RegisterPatch<FriendlistPatch>();
    manager.RegisterPatch<MatchEndCrashFixPatch>();

    manager.ApplyAllPatches();
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        InitializePatches(hModule);
    }
    return TRUE;
}