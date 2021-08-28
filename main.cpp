#include "scene.h"

bool __stdcall DllMain(void* hModule, unsigned long ul_reason_for_call, void* lpReserved)
{
	if (ul_reason_for_call == 1)
	{
		Utilitys::Hooks_Release();
		sigs_initalize();
		return TRUE;
	}
	return 1;
}