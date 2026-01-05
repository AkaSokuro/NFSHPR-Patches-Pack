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
#include "src/patches/DisableBackgroundVideo.h"

PatchManager* g_patchManager = nullptr;
HANDLE g_monitorThread = NULL;
bool g_shouldExit = false;

DWORD WINAPI MonitorThread(LPVOID param) {
    PatchManager* mgr = (PatchManager*)param;

    while (!g_shouldExit) {
        Sleep(1000);
        mgr->ApplyDelayedPatches();
    }

    return 0;
}

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

    g_patchManager = new PatchManager(hProcess, baseAddress, iniPath);

    g_patchManager->RegisterPatch<AffinityFixPatch>();
    g_patchManager->RegisterPatch<GamePriorityPatch>();
    g_patchManager->RegisterPatch<FriendlistPatch>();
    g_patchManager->RegisterPatch<MatchEndCrashFixPatch>();
    g_patchManager->RegisterPatch<DisableBackgroundVideoPatch>();

    g_patchManager->ApplyAllPatches();

    g_monitorThread = CreateThread(NULL, 0, MonitorThread, g_patchManager, 0, NULL);
}

void CleanupPatches() {
    g_shouldExit = true;

    if (g_monitorThread) {
        WaitForSingleObject(g_monitorThread, 2000);
        CloseHandle(g_monitorThread);
        g_monitorThread = NULL;
    }

    if (g_patchManager) {
        delete g_patchManager;
        g_patchManager = nullptr;
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {

    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hModule);
        InitializePatches(hModule);
        break;

    case DLL_PROCESS_DETACH:
        CleanupPatches();
        break;
    }

    return TRUE;
}