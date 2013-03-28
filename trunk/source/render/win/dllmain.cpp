// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include <windows.h>

#include "win_rendercore.h"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	return TRUE;
}

extern "C" __declspec(dllexport) ola::IRenderCore* createRenderCore(HWND hwnd)
{
	ola::IRenderCore* core = new OlaRenderCore();
	core->initialize((unsigned int)hwnd);
	return core;
}

extern "C" __declspec(dllexport) void deleteRenderCore(ola::IRenderCore* core)
{
	core->release();
	delete core;
}


