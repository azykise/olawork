#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>

#include <windows.h>

#include "../interface/ola_engine.h"

#include "window.h"
#include "egl_surface.h"

#include "dll_item.h"

#include "math/ola_math.h"

#define SCREEN_W 800
#define SCREEN_H 600

ola::IRenderCore* core = 0;

ola::IRenderWindow* main_window = 0;
ola::IRenderWindow* second_window = 0;

ola::RenderWindowInfo window_info1;
ola::RenderWindowInfo window_info2;

LRESULT Process_window(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam);

typedef ola::IRenderCore* (*CreateRenderCore)(HWND hWnd);
typedef void (*DeleteD3DRenderCore)(ola::IRenderCore* core);

int _tmain(int argc, _TCHAR* argv[])
{
	olaMath::Init();

	Dll_Item* dll = new Dll_Item("ola_render.dll");

	CreateRenderCore createRenderCore = dll->getFunction<CreateRenderCore>("createRenderCore");
	DeleteD3DRenderCore deleteRenderCore = dll->getFunction<DeleteD3DRenderCore>("deleteRenderCore");

	HWND window0 = create_window("win_demo",0,0,SCREEN_W,SCREEN_H,Process_window);

	core = createRenderCore(window0);

	memset(&window_info1,0,sizeof(ola::RenderWindowInfo));
	window_info1.full = false;
	window_info1.hwnd = window0;
	window_info1.width = SCREEN_W;
	window_info1.height = SCREEN_H;
	strcpy(window_info1.name,"main");
	main_window = core->createWindow(&window_info1);	

	core->setWindow(main_window);

	int bDone = 0;
	MSG sMessage;
	while(!bDone)
	{
        if(PeekMessage(&sMessage, NULL, 0, 0, PM_REMOVE)) {
            if(sMessage.message == WM_QUIT) {
                bDone = 1;
            } else {
                TranslateMessage(&sMessage);
                DispatchMessage(&sMessage);
            }
        }
		
		core->beginDraw();
		core->drawSceneActors();
		core->endDraw();
	}

	core->release();
	deleteRenderCore(core);

	delete dll;

	return 0;
}
#define  lg(...) printf(__VA_ARGS__)
const int vk_w = 87;
const int vk_s = 83;
const int vk_a = 65;
const int vk_d = 68;
const int vk_space = 32;

void key_control(int id)
{
	olaVec3 eye;
	olaVec3 lat;
	ola::vec3 _eye(0,0,0);
	ola::vec3 _lat(0,0,0);
	core->getCamera(0,_eye,_lat);

	eye.Set(_eye.x,_eye.y,_eye.z);
	lat.Set(_lat.x,_lat.y,_lat.z);

	olaVec3 dir = lat - eye;
	dir.Normalize();
	
	static olaVec3 up(0.0f,0.0f,1.0f);

	switch(id)
	{
		case vk_w:
			break;
		case vk_s:
			dir = dir * -1;
			break;
		case vk_a:
			dir = dir.Cross(up) * -1.0f;
			break;;
		case vk_d:
			dir = dir.Cross(up) ;
			break;
		case vk_space:
			//if(core && second_window)
			//{
			//	core->setWindow(second_window);
			//}
			return;
			//lg("campos: %.2f %.2f %.2f",eye.x,eye.y,eye.z);
			//lg("camtag: %.2f %.2f %.2f",lat.x,lat.y,lat.z);
	}

	dir.Normalize();

	eye = eye + dir * 1.5f;
	lat = lat + dir * 1.5f;

	_eye.x = eye.x; _eye.y = eye.y; _eye.z = eye.z;
	_lat.x = lat.x; _lat.y = lat.y; _lat.z = lat.z;

	core->setCamera(0,_eye,_lat);
}

bool mouse_down = false;
int current_x = 0;
int current_y = 0;
int last_x = 0;
int last_y = 0;

void mouse_control(int c_x,int c_y,bool ondown)
{
	if(ondown)
	{
		if(!mouse_down)
		{
			current_x = last_x = c_x;
			current_y = last_y = c_y;
			mouse_down = true;
		}
		else
		{
			current_x = c_x;
			current_y = c_y;
			float dx = current_x - last_x;
			float dy = current_y - last_y;

			last_x = current_x;
			last_y = current_y;

			olaVec3 eye;
			olaVec3 lat;
			ola::vec3 _eye(0,0,0);
			ola::vec3 _lat(0,0,0);
			core->getCamera(0,_eye,_lat);

			eye.Set(_eye.x,_eye.y,_eye.z);
			lat.Set(_lat.x,_lat.y,_lat.z);
			
			olaVec3 dir = lat - eye;
			olaVec3 top = olaMath::IsZero(dir.x) && olaMath::IsZero(dir.y) ? olaVec3(0.0f,1.0f,0.0f) : olaVec3(0.0f,0.0f,1.0f);

			olaVec3 right = dir.Cross(top);
			right.Normalize();
			olaVec3 up = right.Cross(dir);

			olaQuat q1(0,0,0,1);q1.FromAxisAngle(right,dy * 0.1f);
			olaQuat q2(0,0,0,1);q2.FromAxisAngle(up,dx * 0.005f);
			q1.Normalize();
			q2.Normalize();

			olaQuat q = q1 * q2;
			q.Normalize();
			olaMat4 m = q.ToMat4();
			olaVec4 vr(dir.x,dir.y,dir.z,0.0f);
			vr = m * vr;
						
			dir.Set(vr.x,vr.y,vr.z);
			olaVec3 nlat = eye + dir;

			_eye.x = eye.x; _eye.y = eye.y; _eye.z = eye.z;
			_lat.x = nlat.x; _lat.y = nlat.y; _lat.z = nlat.z;

			core->setCamera(0,_eye,_lat);
		}
	}
	else
	{
		mouse_down = false;
	}
}
LRESULT Process_window(HWND hWnd, UINT uiMsg, WPARAM wParam, LPARAM lParam) {
	int y = 0;
	int x = 0;
	switch(uiMsg) {
		case WM_CLOSE:
				PostQuitMessage(0);
				return 0;

		case WM_MOUSEMOVE :	  
			y = (lParam & 0xFFFF0000) >> 16;
			x = (lParam & 0x0000FFFF);
			mouse_control(x,y,wParam & MK_LBUTTON);
			return 0;		
		case WM_KEYDOWN:			
			if( wParam == vk_w ||
				wParam == vk_s ||
				wParam == vk_a ||
				wParam == vk_d ||
				wParam == vk_space)
			{
				
				key_control(wParam);
			}
			return 0;
		case WM_ACTIVATE:
		case WM_KEYUP:
			return 0;
		case WM_SIZE:
			y = (lParam & 0xFFFF0000) >> 16;
			x = (lParam & 0x0000FFFF);
			if(main_window)
			{
				window_info1.width = x;
				window_info1.height = y;
				main_window->resizeRenderWindow(&window_info1);
			}
			return 0;
	}

	return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}