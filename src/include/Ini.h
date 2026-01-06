#pragma once
#include <fstream>

class Ini {
public:
    static bool FileExists(const std::string& path) {
        DWORD attrib = GetFileAttributesA(path.c_str());
        return (attrib != INVALID_FILE_ATTRIBUTES && !(attrib & FILE_ATTRIBUTE_DIRECTORY));
    }

    static bool CreateDefaultIni(const std::string& iniPath) {
        std::ofstream iniFile(iniPath);

        if (!iniFile.is_open()) {
            return false;
        }

        iniFile << "; NFS:HPR Patch - Settings\n";
        iniFile << "; This is a configuration file for NFS:HPR Patches Pack\n";
        iniFile << "\n";

        iniFile << "[CONFIGS]\n";
        iniFile << "# Master switch to enable/disable all patches\n";
        iniFile << "# [ 0 = Disabled, 1 = Enabled (default) ]\n";
        iniFile << "bPatchEnabled=1\n";
        iniFile << "\n";

        iniFile << "[PATCHES]\n";
        iniFile << "# Workaround to fix the same bug found in the 2010 version by disabling CPU 0 affinity on game launch\n";
        iniFile << "# [ 0 = Disabled, 1 = Enabled (default) ]\n";
        iniFile << "bAffinityFix=1\n";
        iniFile << "\n";

        iniFile << "# Set game process priority\n";
        iniFile << "# 1 = Idle, 2 = Below Normal, 3 = Normal (default)\n";
        iniFile << "# 4 = Above Normal, 5 = High, 6 = Realtime (not recommended)\n";
        iniFile << "iGamePriority=3\n";
        iniFile << "\n";

        iniFile << "# Fixes a bug that causes the game to crash when trying to load more than 99 friends in solo careers\n";
        iniFile << "# [ 0 = Disabled, 1 = Enabled (default) ]\n";
        iniFile << "bFriendlistPatch=1\n";
        iniFile << "\n";

        iniFile << "# Fixes a rare crash that may occurs at the end of a match\n";
        iniFile << "# [ 0 = Disabled, 1 = Enabled (default) ]\n";
        iniFile << "bMatchEndCrashFix=1\n";
        iniFile << "\n";

        iniFile << "# Disable background menu video\n";
        iniFile << "# Sometimes the game overlapping the videos causing the frame to drop and stutter\n";
        iniFile << "#     WARNING: Enabling this will cause any video in game to not load\n";
        iniFile << "# [ 0 = Disabled (default), 1 = Enabled ]\n";
        iniFile << "bDisableBackgroundVideo=0\n";
        iniFile << "\n";

        iniFile.close();
        return true;
    }

    static bool EnsureIniExists(const std::string& iniPath) {
        if (!FileExists(iniPath)) {
            return CreateDefaultIni(iniPath);
        }
        return true;
    }
};