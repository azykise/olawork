// dllmain.cpp : ���� DLL Ӧ�ó������ڵ㡣
#include <windows.h>

#include "../impls/ola_rendercore_impl.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	return TRUE;
}

extern "C" __declspec(dllexport) ola::IRenderCore* createRenderCore(HWND hwnd)
{
	//ola::IRenderCore* core = new OlaRenderCore();
	//core->initialize((unsigned int)hwnd);
	//return core;

	return 0;
}

extern "C" __declspec(dllexport) void deleteRenderCore(ola::IRenderCore* core)
{
	core->release();
	delete core;
}


