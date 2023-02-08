#include "stdafx.h"
#include "helper.hpp"

using namespace std;

HMODULE baseModule = GetModuleHandle(NULL);

// INI Variables
bool bAspectFix;
int iAspectFix = 0;
int iCustomResX;
int iCustomResY;
int iInjectionDelay;

// Variables
float fNewX;
float fNewY;
float fNativeAspect = 1.777777791f;
float fPi = 3.14159265358979323846f;
float fNewAspect;
string sExeName;
string sGameName;
string sExePath;

// Aspect Ratio Hook
DWORD64 AspectRatioReturnJMP;
float fAspectRatio;
void __declspec(naked) AspectRatio_CC()
{
    __asm
    {
        cmp [iAspectFix], 1                      // Check if aspect ratio fix enabled
        je aspectratio                           // Jump to aspect ratio fix if enabled
        mov eax, [rbx + 0x00000228]              // Original code
        mov [rdi + 0x2C], eax                    // Original code
        movzx eax, byte ptr [rbx + 0x0000022C]   // Original code
        jmp [AspectRatioReturnJMP]

    aspectratio:
        mov eax, fNewAspect                      // Copy new aspect ratio to eax
        mov [rdi + 0x2C], eax                    // Original code
        movzx eax, byte ptr [rbx + 0x0000022C]   // Original code
        jmp [AspectRatioReturnJMP]
    }
}

void Logging()
{
    loguru::add_file("HogwartsLegacyFix.log", loguru::Truncate, loguru::Verbosity_MAX);
    loguru::set_thread_name("Main");
}

void ReadConfig()
{
    // Initialize config
    // UE4 games use launchers so config path is relative to launcher
    INIReader config(".\\Phoenix\\Binaries\\Win64\\HogwartsLegacyFix.ini");

    // Check if config failed to load
    if (config.ParseError() != 0) {
        LOG_F(ERROR, "Can't load config file");
        LOG_F(ERROR, "Parse error: %d", config.ParseError());
    }

    iInjectionDelay = config.GetInteger("HogwartsLegacyFix Parameters", "InjectionDelay", 2000);
    bAspectFix = config.GetBoolean("Fix Aspect Ratio", "Enabled", true);

    // Get game name and exe path
    LPWSTR exePath = new WCHAR[_MAX_PATH];
    GetModuleFileNameW(baseModule, exePath, _MAX_PATH);
    wstring exePathWString(exePath);
    sExePath = string(exePathWString.begin(), exePathWString.end());
    wstring wsGameName = Memory::GetVersionProductName();
    sExeName = sExePath.substr(sExePath.find_last_of("/\\") + 1);
    sGameName = string(wsGameName.begin(), wsGameName.end());

    LOG_F(INFO, "Game Name: %s", sGameName.c_str());
    LOG_F(INFO, "Game Path: %s", sExePath.c_str());

    // Custom resolution
    if (iCustomResX > 0 && iCustomResY > 0)
    {
        fNewX = (float)iCustomResX;
        fNewY = (float)iCustomResY;
        fNewAspect = (float)iCustomResX / (float)iCustomResY;
    }

    // Grab desktop resolution
    RECT desktop;
    GetWindowRect(GetDesktopWindow(), &desktop);
    fNewX = (float)desktop.right;
    fNewY = (float)desktop.bottom;
    fNewAspect = (float)desktop.right / (float)desktop.bottom;

    // Log config parse
    LOG_F(INFO, "Config Parse: iInjectionDelay: %dms", iInjectionDelay);
    LOG_F(INFO, "Config Parse: bAspectFix: %d", bAspectFix);
    LOG_F(INFO, "Config Parse: iCustomResX: %d", iCustomResX);
    LOG_F(INFO, "Config Parse: iCustomResY: %d", iCustomResY);
    LOG_F(INFO, "Config Parse: fNewX: %.2f", fNewX);
    LOG_F(INFO, "Config Parse: fNewY: %.2f", fNewY);
    LOG_F(INFO, "Config Parse: fNewAspect: %.4f", fNewAspect);
}

void AspectFix()
{
    if (bAspectFix)
    {
        iAspectFix = 1;
        uint8_t* AspectRatioScanResult = Memory::PatternScan(baseModule, "F3 0F ?? ?? ?? ?? ?? ?? F3 0F ?? ?? ?? 8B ?? ?? ?? ?? ?? 89 ?? ?? 0F ?? ?? ?? ?? ?? ?? 33 ?? ?? 83 ?? ??");
        if (AspectRatioScanResult)
        {
            DWORD64 AspectRatioAddress = ((uintptr_t)AspectRatioScanResult + 0xD);
            int AspectRatioHookLength = Memory::GetHookLength((char*)AspectRatioAddress, 14);
            AspectRatioReturnJMP = AspectRatioAddress + AspectRatioHookLength;
            Memory::DetourFunction64((void*)AspectRatioAddress, AspectRatio_CC, AspectRatioHookLength);

            LOG_F(INFO, "Aspect Ratio: Hook length is %d bytes", AspectRatioHookLength);
            LOG_F(INFO, "Aspect Ratio: Hook address is 0x%" PRIxPTR, (uintptr_t)AspectRatioAddress);
        }
        else if (!AspectRatioScanResult)
        {
            LOG_F(INFO, "Aspect Ratio: Pattern scan failed.");
        }
    }
}

DWORD __stdcall Main(void*)
{
    Logging();
    ReadConfig();
    AspectFix();
    return true; // end thread
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        CreateThread(NULL, 0, Main, 0, NULL, 0);
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

