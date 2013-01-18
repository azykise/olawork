/*
 * This proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 * (C) COPYRIGHT 2009 - 2011 ARM Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 */

#ifndef WINDOW_H
#define WINDOW_H

#include <windows.h>

typedef LRESULT (*window_event)(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);

HWND create_window(const char* win_name,int x,int y,int uiWidth, int uiHeight,window_event handle);

#endif
