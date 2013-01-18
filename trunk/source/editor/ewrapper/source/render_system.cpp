#include "pre_define.h"
#include "render_system.h"

const char* enginedll_d_path = "ola_render.dll";
const char* enginedll_path = "ola_render.dll";

const char* create_rendercore = "createRenderCore"; //createD3DRenderCore
const char* delete_rendercore = "deleteRenderCore"; //deleteD3DRenderCore

namespace Engine
{
	using namespace Tool;
	RenderWindow::RenderWindow(IRenderWindow* wnd)
	{
		m_window = wnd;
	}

	RenderWindow::~RenderWindow()
	{
		m_window = 0;
	}

	void RenderWindow::resetWindow(int w,int h,System::IntPtr hwnd)
	{
		RenderWindowInfo info = {0};
		info.height = h;
		info.width = w;
		info.hwnd = hwnd.ToInt32();
		info.full = false;
		m_window->resizeRenderWindow(&info);
	}

	void RenderWindow::setBackgroundColor(float r,float g,float b,float a)
	{
		m_window->setBackgroundColor(r,g,b,a);
	}

	RenderSystem::RenderSystem(System::IntPtr hwnd):m_engine_dll(0),RenderCore(0),m_view_param(0)
	{
		m_engine_dll = new Dll_Item(enginedll_d_path);

		typedef IRenderCore* (*CreateD3DRenderCore)(HWND);
		CreateD3DRenderCore create_render = m_engine_dll->getFunction<CreateD3DRenderCore>(create_rendercore);
		RenderCore = create_render((HWND)(hwnd.ToInt32()));

		if(!RenderCore)
			throw gcnew System::Exception("渲染系统创建失败");

		m_view_param = new tViewParam();
	}

	RenderSystem::~RenderSystem()
	{
		if(m_engine_dll)
		{
			delete m_engine_dll;
			m_engine_dll = 0;
		}

		if(m_view_param)
		{
			delete m_view_param;
			m_view_param = 0;
		}
	}

	void RenderSystem::release()
	{
		System::GC::Collect();
		System::GC::WaitForFullGCComplete();

		if (RenderCore != 0)
		{
			typedef  void (*DeleteD3DRenderCore)(IRenderCore*);
			DeleteD3DRenderCore delete_render = m_engine_dll->getFunction<DeleteD3DRenderCore>(delete_rendercore);
			delete_render(RenderCore);
			RenderCore = 0;
		}
	}


	RenderWindow^ RenderSystem::createWindow(int w,int h,System::IntPtr hwnd)
	{
		RenderWindowInfo info = {0};
		info.height = h;
		info.width = w;
		info.hwnd = hwnd.ToInt32();
		info.full = false;
		IRenderWindow* iwnd = RenderCore->createWindow(&info);
		RenderWindow^ wnd = gcnew RenderWindow(iwnd);
		return wnd;
	}

	void RenderSystem::setRenderWindow(RenderWindow^ wnd)
	{
		RenderCore->setWindow(wnd->getWindow());
	}

	void RenderSystem::setViewParament(ViewParament^ param)
	{
		m_view_param->aspect	= param->Aspect;
		m_view_param->far_dist	= param->FarDist;
		m_view_param->near_dist = param->NearDist;
		m_view_param->fovy		= param->Fovy;

		arrayToUnSafe(param->LookAtPt,m_view_param->lookat);
		arrayToUnSafe(param->Position,m_view_param->pos);

		RenderCore->setView(m_view_param);
	}

	Matrix4F^ RenderSystem::getViewMatrix()
	{
		float f[16];
		RenderCore->getViewMatrix(f);
		array<float>^ mat_arr = unsafeToArray(f,16);
		Matrix4F^ mat = gcnew Matrix4F(mat_arr);
		return mat;
	}

	Matrix4F^ RenderSystem::getProjMatrix()
	{
		float f[16];
		RenderCore->getProjMatrix(f);
		array<float>^ mat_arr = unsafeToArray(f,16);
		Matrix4F^ mat = gcnew Matrix4F(mat_arr);
		return mat;
	}

	PrimitiveLine^ RenderSystem::createPrimitiveLine(Vector3F^ start,Vector3F^ end)
	{		
		IPrimitive*	p = RenderCore->createPrimitive(PMT_LINE);

		PrimitiveLine^ _line = gcnew PrimitiveLine(p,this);
		_line->setLine(start,end);
		return _line;
	}

	PrimitiveGrid^ RenderSystem::createPrimitiveGrid(Vector3F^ center,int grid_hnum,int grid_vnum,int space_h,int space_v)
	{
		IPrimitive*	p = RenderCore->createPrimitive(PMT_LINE);

		PrimitiveGrid^ grid = gcnew PrimitiveGrid(p,this);
		grid->setGrid(center,grid_hnum,grid_vnum,space_h,space_v);
		return grid;
	}

	PrimitiveGrid^ RenderSystem::createPrimitiveGrid()
	{
		IPrimitive*	p = RenderCore->createPrimitive(PMT_LINE);
		PrimitiveGrid^ grid = gcnew PrimitiveGrid(p,this);
		return grid;
	}

	PrimitiveBorderCube^ RenderSystem::createPrimitiveBorderCube(Vector3F^ center,float width,float height,float length)
	{
		IPrimitive*	p = RenderCore->createPrimitive(PMT_LINE);	

		PrimitiveBorderCube^ cube = gcnew PrimitiveBorderCube(p,this);
		cube->setBorderCube(center,width,height,length);		
		return cube;
	}

	PrimitiveCircle^ RenderSystem::createPrimitiveCircle(Vector3F^ center,Vector3F^ normal,float radius)
	{
		IPrimitive* p = RenderCore->createPrimitive(PMT_LINE);
		PrimitiveCircle^ circle = gcnew PrimitiveCircle(p,this);
		circle->setCircle(center,normal,radius);
		return circle;
	}

	PrimitivePlane^ RenderSystem::createPrimitivePlane(Vector3F^ center,Vector3F^ normal,float w,float h)
	{
		IPrimitive* p = RenderCore->createPrimitive(PMT_LINE);
		PrimitivePlane^ plane = gcnew PrimitivePlane(p,this);
		plane->setPlane(center,normal,w,h);
		return plane;
	}

	StaticModel^ RenderSystem::createStaticModel(System::String^ filename)
	{
		IStaticModel* model = RenderCore->createStaticModel(Tool::getStringPtr(filename));
		if(model)
		{
			StaticModel^ smodel = gcnew StaticModel(filename,model,this);
			return smodel;
		}
		return nullptr;
	}

	DynamicModel^ RenderSystem::createDynamicModel(System::String^ filename)
	{
		ICharacter* chr = RenderCore->createCharacter(Tool::getStringPtr(filename));
		if (chr)
		{
			DynamicModel^ dy = gcnew DynamicModel(filename,chr,this);
			return dy;
		}
		return nullptr;
	}

	Light^ RenderSystem::createLight( System::String^ name )
	{
		ILight* light = RenderCore->createLight(Tool::getStringPtr(name));
		if (light)
		{
			Light^ l = gcnew Light(name,light,this);
			return l;
		}
		return nullptr;
	}

	Scene^ RenderSystem::createSmallScene( System::String^ name )
	{
		ola::IScene* s = RenderCore->createScene(Tool::getStringPtr(name),IScene::SCENE_SMALL);
		Scene^ scene = gcnew Scene(name,s,this);
		return scene;
	}

	void RenderSystem::setRenderScene( Scene^ scene )
	{
		RenderCore->setRenderingScene(scene->scene());
	}

	void RenderSystem::pushToRender(Primitive^ p)
	{
		RenderCore->pushRender(p->getPrimitive());
	}

	void RenderSystem::beginDraw()
	{		
		RenderCore->beginDraw();
	}

	void RenderSystem::endDraw()
	{
		RenderCore->endDraw();
	}

	void RenderSystem::drawSceneActors()
	{
		RenderCore->drawSceneActors();
	}

	void RenderSystem::pushToSelect(Primitive^ p,unsigned int sel_id)
	{
		RenderCore->pushHardwareSelect(p->getPrimitive(),sel_id);
	}

	void RenderSystem::beginHardwareSelect(int x,int y,int w,int h)
	{
		RenderCore->beginHardwareSelect(x,y,w,h);
	}

	int RenderSystem::endHardwareSelect()
	{
		int num = 0;
		RenderCore->endHardwareSelect(num);
		return num;
	}

	array<unsigned int>^ RenderSystem::getSelectResult(int count)
	{
		array<unsigned int>^ result = gcnew array<unsigned int>(count);
		//if (count > 0)
		//{
		//	unsigned int* res = m_render_core->getSelectedResult();
		//	for (int i = 0 ; i < count ; i++)
		//	{
		//		result[i] = res[i];
		//	}
		//}
		return result;
	}

	void RenderSystem::clearResourceCache( System::String^ type )
	{
		RenderCore->clearResourceCache("");
	}

	int RenderSystem::getResourceNum( System::String^ type )
	{
		return RenderCore->getResourceNum("");
	}
}