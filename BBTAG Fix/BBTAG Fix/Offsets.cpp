#include "Include.h"

Offsets offsets;

void Offsets::Initialize()
{
	hModule = GetModuleHandleA(NULL);
	if (hModule)
	{
		pGetSteamInterfaces = PatternScan(hModule, "55 8B EC 8B 4D 08 E8 ? ? ? ? FF 15 ? ? ? ?");
		if (!pGetSteamInterfaces)
			MessageBoxA(NULL, "Couldn't find GetSteamInterfaces function by signature.", "BBTAG Fix - Error", MB_OK);
	}

	else
		MessageBoxA(NULL, "Couldn't retrieve module handle on main module.", "BBTAG Fix - Error", MB_OK);
}