#include "pch.h"
#include <iostream>

void PrintHello() {
	std::cout << "Hello DLL!\n";
}

int Add(int i1, int i2) {
	return i1 - i2;
}

void SetupHooks() {
	// Make the .text segment writeable
	DWORD originalProtection;
	VirtualProtectEx(GetCurrentProcess(), (LPVOID)0x00401000, 0x000075ff, PAGE_EXECUTE_READWRITE, &originalProtection);

	// Addresses of the original functions
	uintptr_t OriginalAddAddress = 0x00412200;
	uintptr_t OriginalPrintHelloAddress = 0x00412250;

	// Get the addresses of the re-implemented functions
	uintptr_t DllAddAddress = reinterpret_cast<uintptr_t>(Add);
	uintptr_t DllPrintHelloAddress = reinterpret_cast<uintptr_t>(PrintHello);

	// Calculate the offset between the original and re-implemented functions
	uintptr_t OffsetAdd = DllAddAddress - OriginalAddAddress - 5;
	uintptr_t OffsetPrintHello = DllPrintHelloAddress - OriginalPrintHelloAddress - 5;

	// Jump instruction
	const char Jump = (char)0xE9;

	// Hook the Add function
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)OriginalAddAddress, &Jump, 1, nullptr);
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)(OriginalAddAddress + 1), &OffsetAdd, 4, nullptr);

	// Hook the PrintHello function
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)OriginalPrintHelloAddress, &Jump, 1, nullptr);
	WriteProcessMemory(GetCurrentProcess(), (LPVOID)(OriginalPrintHelloAddress + 1), &OffsetPrintHello, 4, nullptr);

}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		SetupHooks();
		break;

	default:
		break;
	}

	return TRUE;
}
