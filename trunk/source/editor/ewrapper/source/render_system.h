#pragma once

#include "pre_define.h"
#include "../../../utility/dll_item.h"

#include "model.h"
#include "light.h"
#include "scene.h"
#include "primitive.h"

namespace Engine
{
	public ref class RenderWindow
	{
	public:
		RenderWindow(IRenderWindow* wnd);
		~RenderWindow();

		void resetWindow(int w,int h,System::IntPtr hwnd);

		void setBackgroundColor(float r,float g,float b,float a);

		int getWidth()	{return m_window->getWidth();};
		int getHeight() {return m_window->getHeight();};


		IRenderWindow*  getWindow(){return m_window;}

	protected:
		IRenderWindow*  m_window;
	};

	public ref class ViewParament
	{
	public:
		ViewParament()
		{
			Position = gcnew array<float>(3);
			LookAtPt = gcnew array<float>(3);

			Position[0] = Position[1] = Position[2] = 0.0f;
			LookAtPt[0] = LookAtPt[1] = LookAtPt[2] = 0.0f;

			Aspect = 4.0f/3.0f;
			Fovy = (float)(System::Math::PI/4);
			NearDist = 1.0f;
			FarDist = 1001.0f;
		}
		array<float>^ Position;
		array<float>^ LookAtPt;
		float Fovy;
		float Aspect;
		float NearDist;
		float FarDist;	
	};

	public ref class RenderSystem
	{
	public:
		RenderSystem(System::IntPtr hwnd);
		~RenderSystem();

		void release();

		RenderWindow^ createWindow(int w,int h,System::IntPtr hwnd);
		void setRenderWindow(RenderWindow^ wnd);
		
		PrimitiveLine^ createPrimitiveLine(Vector3F^ start,Vector3F^ end); 
		PrimitiveGrid^ createPrimitiveGrid();
		PrimitiveGrid^ createPrimitiveGrid(Vector3F^ center,int grid_hnum,int grid_vnum,int space_h,int space_v);
		PrimitiveBorderCube^ createPrimitiveBorderCube(Vector3F^ center,float width,float height,float length);		
		PrimitiveCircle^ createPrimitiveCircle(Vector3F^ center,Vector3F^ normal,float radius);
		PrimitivePlane^ createPrimitivePlane(Vector3F^ center,Vector3F^ normal,float w,float h);

		StaticModel^ createStaticModel(System::String^ filename);
		DynamicModel^ createDynamicModel(System::String^ filename);
		Light^ createLight(System::String^ name);
		Scene^ createSmallScene(System::String^ name);

		//目前只有一个一个视锥 以后可能需要将视锥与window或者rt关联
		void setViewParament(ViewParament^ param);
		Matrix4F^ getViewMatrix();
		Matrix4F^ getProjMatrix();

		void setRenderScene(Scene^ scene);

		void pushToRender(Primitive^ p);
		void beginDraw();
		void drawSceneActors();
		void endDraw();		

		void pushToSelect(Primitive^ p,unsigned int sel_id);
		void beginHardwareSelect(int x,int y,int w,int h);
		int  endHardwareSelect();
		array<unsigned int>^ getSelectResult(int count);

		void clearResourceCache(System::String^ type);
		int getResourceNum(System::String^ type);

		IRenderCore* RenderCore;
	protected:
		Dll_Item* m_engine_dll;		
		tViewParam* m_view_param;
	};
}