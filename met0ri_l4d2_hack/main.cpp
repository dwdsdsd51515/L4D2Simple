
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
using namespace std;

#include "sdk.h"

#include "utils.h"
#include "vfunc_hook.h"
#include "offsets.h"
#include "interfaces.h"
#include "hooks.h"

void alloc_console()
{
#ifdef _DEBUG
	AllocConsole();
	freopen("conin$", "r", stdin);
	freopen("conout$", "w", stdout);
	freopen("conout$", "w", stderr);
	SetConsoleTitleA("met0ri-csgo by user344 v0.1");
#endif
}

void free_console()
{
	FreeConsole();
}


BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		alloc_console();
		offsets::initialize();
		capture_interfaces();
		//dump_netvars();
		initialize_hooks();

		return true;
	}
	if (fdwReason == DLL_PROCESS_DETACH)
	{
		free_console();
		//unhook_vtables();
		//features::unload();
		//renderer::unload();
	}

	return false;
}