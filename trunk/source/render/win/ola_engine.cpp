#include "win_rendercore.h"

#define DLL_CEXPORT extern "C" __declspec(dllexport)
typedef unsigned int uint;

DLL_CEXPORT float* RenderCore_MallocFloats(unsigned int size)
{
	float* fs = new float[size];
	return fs;
}

DLL_CEXPORT void RenderCore_FreeFloats(float* fs)
{
	delete[] fs;
}

DLL_CEXPORT int* RenderCore_MallocInts(unsigned int size)
{
	int* ints = new int[size];
	return ints;
}

DLL_CEXPORT void RenderCore_FreeInts(int* ints)
{
	delete[] ints;
}

DLL_CEXPORT uint RenderCore_Initialize(ola::IRenderCore* core, unsigned int hwnd)
{
	uint r = core->initialize(hwnd) ? 1 : 0;
	return 0;
}

DLL_CEXPORT void RenderCore_Release(ola::IRenderCore* core)
{
	core->release();
}

DLL_CEXPORT void RenderCore_BeginDraw(ola::IRenderCore* core)
{
	core->beginDraw();
}

DLL_CEXPORT void RenderCore_EndDraw(ola::IRenderCore* core)
{
	core->endDraw();
}

DLL_CEXPORT void RenderCore_DrawSceneActors(ola::IRenderCore* core)
{
	core->drawSceneActors();
}

DLL_CEXPORT void RenderCore_ReleaseObject(ola::IRenderCore* core,ola::IObject* object)
{
	core->releaseObject(object);
} 

DLL_CEXPORT ola::IRenderWindow* RenderCore_CreateWindow(
	ola::IRenderCore* core, unsigned int hwnd ,int w,int h)
{
	ola::RenderWindowInfo info;
	info.hwnd = hwnd;
	info.width = w;
	info.height = h;

	return core->createWindow(&info);
}

DLL_CEXPORT void RenderCore_SetWindow(ola::IRenderCore* core,ola::IRenderWindow* wnd)
{
	core->setWindow(wnd);
}

DLL_CEXPORT void RenderCore_SetView(ola::IRenderCore* core,
	float px,float py,float pz,
	float lx,float ly,float lz,
	float fovy,float aspect,float n,float f)
{
	ola::tViewParam vp;
	vp.pos[0] = px;vp.pos[1] = py;vp.pos[2] = pz;
	vp.lookat[0] = lx;vp.lookat[1] = ly;vp.lookat[2] = lz;
	vp.aspect = aspect;
	vp.fovy = fovy;
	vp.near_dist = n;
	vp.far_dist = f;

	core->setView(&vp);
}

DLL_CEXPORT void RenderCore_GetView(ola::IRenderCore* core,
	float* px,float* py,float* pz,
	float* lx,float* ly,float* lz,
	float* fovy,float* aspect,float* n,float* f)
{
	ola::tViewParam vp;
	core->getView(&vp);

	*px = vp.pos[0]; *py = vp.pos[1];*pz = vp.pos[2];
	*lx = vp.lookat[0];*ly = vp.lookat[1];*lz = vp.lookat[2];
	*aspect = vp.aspect;
	*fovy = vp.fovy;
	*n = vp.near_dist;
	*f = vp.far_dist;
}

DLL_CEXPORT void RenderCore_GetViewMatrix(ola::IRenderCore* core,float* f44)
{
	core->getViewMatrix(f44);
}

DLL_CEXPORT void RenderCore_GetProjMatrix(ola::IRenderCore* core,float* f44)
{
	core->getProjMatrix(f44);
}

DLL_CEXPORT ola::IPrimitive* RenderCore_CreatePrimitive(ola::IRenderCore* core)
{
	return core->createPrimitive(ola::PrimitiveType::PMT_LINE);
}

DLL_CEXPORT void RenderCore_PushPrimitive(ola::IRenderCore* core,ola::IPrimitive* p)
{
	core->pushRender(p);
}

DLL_CEXPORT void Primitive_SetVertex(ola::IPrimitive* prim,float* vec3s,int length)
{
	prim->setVertex(vec3s,length);
}

DLL_CEXPORT void Primitive_SetIndex(ola::IPrimitive* prim,int* idxs,int length)
{
	prim->setIndex(idxs,length);
}

//
//DLL_CEXPORT void RenderCore_Release(ola::IRenderCore* core)
//{
//	core->release();
//}