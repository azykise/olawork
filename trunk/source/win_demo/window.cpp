/*
 * This proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 * (C) COPYRIGHT 2009 - 2011 ARM Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 */

/* 
 * window.c
 * Platform-specific window setup.
 */

#include "window.h"

/* 
 * create_window(): Set up Windows specific bits.
 *
 * uiWidth:	 Width of window to create.
 * uiHeight:	Height of window to create.
 *
 * Returns:	 Device specific window handle.
 */
LRESULT process_window(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);

HWND create_window(const char* win_name,int x,int y,int uiWidth, int uiHeight,window_event handle) {
	WNDCLASS wc;
	RECT wRect;
	HWND sWindow;
	HINSTANCE hInstance;

	wRect.left = (long)x;
	wRect.right = (long)uiWidth;
	wRect.top = (long)y;
	wRect.bottom = (long)uiHeight;

	hInstance = GetModuleHandle(NULL);

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = handle == 0 ? (WNDPROC)process_window :(WNDPROC)handle;//process_window;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "OGLES";

	RegisterClass(&wc);

	AdjustWindowRectEx(&wRect, WS_OVERLAPPEDWINDOW, FALSE, WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);

	sWindow = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE, "OGLES", win_name, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, x, y, uiWidth, uiHeight, NULL, NULL, hInstance, NULL);

	ShowWindow(sWindow, SW_SHOW);
	SetForegroundWindow(sWindow);
	SetFocus(sWindow);

	return sWindow;
}

LRESULT process_window(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}


