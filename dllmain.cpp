// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "mem.h"
#include "proc.h"
#include <iostream>
#include <TlHelp32.h>
#include <Windows.h>


#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}

class weapon
{
public:
    union {
        //              Type     Name    Offset
        DEFINE_MEMBER_N(int32_t*, ammoPtr, 0x14);
        DEFINE_MEMBER_N(weapon*, currentWeapon, 0x0378);
    };
};

class ent
{
public:
    // be sure to put all the offsets you pad this way in a union
    union
    {
        //              Type     Name    Offset
        DEFINE_MEMBER_N(int32_t, health, 0xF8);
        DEFINE_MEMBER_N(int32_t, armor, 0xFC);
        DEFINE_MEMBER_N(int32_t, grenades, 0x158);
        DEFINE_MEMBER_N(int32_t, grenadesCooldown, 0x180);
        DEFINE_MEMBER_N(int32_t, knifeCooldown, 0x160);
        DEFINE_MEMBER_N(int32_t, pistolCooldown, 0x164);
        DEFINE_MEMBER_N(int32_t, carbineCooldown, 0x168);
        DEFINE_MEMBER_N(int32_t, shotgunCooldown, 0x16C);
        DEFINE_MEMBER_N(int32_t, smgCooldown, 0x170);
        DEFINE_MEMBER_N(int32_t, sniperCooldown, 0x174);
        DEFINE_MEMBER_N(int32_t, assaultCooldown, 0x178);
        DEFINE_MEMBER_N(weapon*, currentWeapon, 0x378);
    };
};

typedef char*(__cdecl * _ACPrintF)(char * sFormat, ...);
_ACPrintF ACPrintF;

DWORD WINAPI HackThread(HMODULE hModule)
{
    // Create Console
    AllocConsole();
    FILE* f;
    freopen_s(&f, "CONOUT$", "w", stdout);

    std::cout << "stubudubu in ur memz\n" << std::endl;

    // Get Module Base Address

    uintptr_t moduleBase = (uintptr_t)GetModuleHandle(L"ac_client.exe");
    
    //My original acprint offset = 0x6B147
    
    ACPrintF = (_ACPrintF)(moduleBase + 0x6B060);

    const char* sFormat = "%s";
    char* sFormatMutable = const_cast<char*>(sFormat);

    bool bRecoil = false;

    //std::cout << *(int*)(moduleBase + 0x10f4f4 + 0xf8) << std::endl; // Print health


   // std::cout << *(int*)(moduleBase + 0x10f4f4 + 0x374 + 0x14 + 0x0); // print ammo

    // Write to memory in loop, so froze values

    ACPrintF(sFormatMutable, "Hack Loading"); // Print to game message box
    Sleep(2000);
    ACPrintF(sFormatMutable, "Hack Loaded");

    ent* localPlayerPtr = *(ent**)(moduleBase + 0x10F4F4);

    // print health & ammo status pre hack

    ACPrintF(sFormatMutable, "Setting health...");
    ACPrintF(sFormatMutable, "Setting Ammo...");
    ACPrintF(sFormatMutable, "Negating Cooldowns...");
    ACPrintF(sFormatMutable, "Press END to unload hack");

    while (moduleBase)
    {
        //ent* localPlayerPtr = *(ent**)(moduleBase + 0x10F4F4);

        if (localPlayerPtr)
        {

            localPlayerPtr->health = 6969;
            localPlayerPtr->armor = 1337;
            localPlayerPtr->grenades = 69;
            localPlayerPtr->grenadesCooldown = 0;
            localPlayerPtr->knifeCooldown = 0;
            localPlayerPtr->pistolCooldown = 0;
            localPlayerPtr->carbineCooldown = 0;
            localPlayerPtr->sniperCooldown = 0;
            localPlayerPtr->shotgunCooldown = 0;
            localPlayerPtr->smgCooldown = 0;
            localPlayerPtr->assaultCooldown = 0;
            *localPlayerPtr->currentWeapon->ammoPtr = 1337;

        }

        if (GetAsyncKeyState(VK_END) & 1) // Come out of loop, eject safe
        {
			break;
		}

        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            bRecoil = !bRecoil;

            if (bRecoil)
            {
                // nop the recoil function
                mem::Nop((BYTE*)(moduleBase + 0x63786), 10);

            }

            else
            {
                //write back original instruction if toggled
                mem::Patch((BYTE*)(moduleBase + 0x63786), (BYTE*)"\x50\x8d\x4c\x24\x1c\x51\x8b\xce\xff\xd2", 10);
            }
        }
    }
    Sleep(5);
    
    // Cleanup & Eject
    fclose(f);
    FreeConsole();
    FreeLibraryAndExitThread(hModule, 0);
	return 0;

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
        CloseHandle(CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)HackThread, hModule, 0, nullptr));
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

