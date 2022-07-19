#pragma once

inline std::uintptr_t ReadPointer(std::uintptr_t address, std::uintptr_t offset)
{
	if (!address)
		return 0;

	if (sizeof(std::uintptr_t) == 0x8)
		return address + (int)((*(int*)(address + offset) + offset) + sizeof(int));

	return *(std::uintptr_t*)(address + offset);
}

inline std::vector<int> PatternToBytes(const char* pattern)
{
	auto bytes = std::vector<int>{};
	auto start = (char*)pattern;
	auto end = (char*)pattern + strlen(pattern);

	for (auto current = start; current < end; ++current)
	{
		if (*current == '?')
		{
			++current;
			if (*current == '?')
				++current;
			bytes.push_back(-1);
		}
		else
			bytes.push_back(strtoul(current, &current, 16));
	}
	return bytes;
};

inline std::uintptr_t PatternScan(HMODULE hModule, const char* signature, std::uintptr_t offset = 0, bool dereference = false)
{
	auto dosHeader = (PIMAGE_DOS_HEADER)hModule;
	auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)hModule + dosHeader->e_lfanew);

	auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
	auto patternBytes = PatternToBytes(signature);
	auto scanBytes = reinterpret_cast<std::uint8_t*>(hModule);

	auto size = patternBytes.size();
	auto data = patternBytes.data();

	for (size_t i = 0; i < sizeOfImage - size; ++i)
	{
		bool found = true;
		for (size_t j = 0; j < size; ++j)
		{
			if (scanBytes[i + j] != data[j] && data[j] != -1)
			{
				found = false;
				break;
			}
		}

		if (found)
			return dereference ? ReadPointer((std::uintptr_t)&scanBytes[i], offset) : (std::uintptr_t)&scanBytes[i];
	}

	return 0;
}

inline std::uintptr_t HookVMT(std::uintptr_t pVtable, std::uintptr_t hook, unsigned int index)
{
	auto vtable = *(std::uintptr_t**)pVtable;
	std::uintptr_t backup = 0;
	MEMORY_BASIC_INFORMATION mbi{};
	VirtualQuery((LPCVOID)vtable, &mbi, sizeof(mbi));
	VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_READWRITE, &mbi.Protect);
	backup = vtable[index];
	vtable[index] = (std::uintptr_t)hook;
	VirtualProtect(mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &mbi.Protect);
	return backup;
}