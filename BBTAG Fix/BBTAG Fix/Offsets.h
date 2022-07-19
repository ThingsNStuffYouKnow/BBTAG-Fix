#pragma once

class Offsets
{
public:
	void Initialize();

	HMODULE hModule;
	std::uintptr_t pGetSteamInterfaces;
};

extern Offsets offsets;