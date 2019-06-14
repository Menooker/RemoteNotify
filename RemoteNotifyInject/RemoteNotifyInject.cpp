// RemoteNotifyInject.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include <Windows.h>
#include <assert.h>
#include <stdio.h>
#include <string>

#pragma data_seg("SHARED")
HWND hTargetWnd = nullptr;
bool hasMsg = false;
HHOOK hHook = nullptr;
#pragma data_seg()
#pragma comment(linker, "/section:SHARED,RWS") 

bool isMaster = false;
HMODULE hMyModule;
LRESULT CALLBACK MsgFilter(int code, WPARAM wparam, LPARAM lparam)
{
	auto pmsg = (PMSG)lparam;
	if (pmsg->hwnd == hTargetWnd && pmsg->message == WM_TIMER)
	{
		if(pmsg->wParam == 1)
			hasMsg = true;
	}
	return CallNextHookEx(hHook, code, wparam, lparam);
}

__declspec(dllexport) HHOOK Inject(HWND hWnd)
{
	DWORD dummy;
	hTargetWnd = hWnd;
	auto ret = SetWindowsHookExA(WH_GETMESSAGE, MsgFilter, hMyModule, GetWindowThreadProcessId(hWnd, &dummy));
	assert(ret);
	hHook = ret;
	isMaster = true;
	return ret;
}

__declspec(dllexport) bool HasMessage()
{
	auto ret = hasMsg;
	hasMsg = false;
	return ret;
}

__declspec(dllexport) void UnInject()
{
	printf("Exiting\n");
	if(hHook)
		UnhookWindowsHookEx(hHook);
	hHook = nullptr;
}

extern HMODULE hMyModule;
BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	hMyModule = hModule;
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		if (isMaster)
			UnInject();
		break;
	}
	return TRUE;
}