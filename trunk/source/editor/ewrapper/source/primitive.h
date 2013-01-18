#pragma once

#include "pre_define.h"

namespace Engine
{
	ref class RenderSystem;
	public ref class Primitive
	{
	public:
		Primitive(IPrimitive* p,RenderSystem^ rendersys);
		virtual ~Primitive();
		!Primitive();

		virtual void render(RenderSystem^ rendersys);
		
		virtual void setColor(float r,float g,float b,float a);

		IPrimitive* getPrimitive()	{return m_primitive;} 

		static float computeDottedSpace(float line_length);

		System::UInt64 getID(){return m_id;}
	
		RenderSystem^ getRenderSystem() {return m_rendersys;}

	protected:
		RenderSystem^ m_rendersys;
		System::UInt64 m_id;
		IPrimitive* m_primitive;
		array<float>^ m_data;
	};

	public ref class PrimitiveLine : public Primitive
	{
	public:
		PrimitiveLine(IPrimitive* p,RenderSystem^ rendersys);

		void setLine(float x0,float y0,float z0,float x1,float y1,float z1);

		void setLine(Vector3F^ start,Vector3F^ end);

	protected:
		Vector3F^ m_start;
		Vector3F^ m_end;
	};

	public ref class PrimitiveGrid : public Primitive
	{
	public:
		PrimitiveGrid(IPrimitive* p,RenderSystem^ rendersys);
		virtual ~PrimitiveGrid();

		void setGrid(Vector3F^ center,int grid_hnum,int grid_vnum,int space_h,int space_v);

		//À≥ ±’Î
		void setRect(Vector3F^ v0,Vector3F^ v1,Vector3F^ v2,Vector3F^ v3);

	protected:
		Vector3F^ m_center;
		int m_grid_hnum,m_grid_vnum;
		float m_space_h,m_space_v;
		float* m_rectpt;
	};

	public ref class PrimitiveBorderCube : public Primitive
	{
	public:
		PrimitiveBorderCube(IPrimitive* p,RenderSystem^ rendersys);

		void setAsAABBCube(Vector3F^ min,Vector3F^ max);

		void setBorderCube(Vector3F^ center,float w,float h,float l);

	protected:
		Vector3F^ m_center;
		float m_width;
		float m_height;
		float m_length;
	};

	public ref class PrimitiveCube : public Primitive
	{
	public:
		PrimitiveCube(IPrimitive* p,RenderSystem^ rendersys);

		void setCube(Vector3F^ center,float size);

	protected:
		Vector3F^ m_center;
		float m_size;
	};

	public ref class PrimitiveCircle : public Primitive
	{
	public:
		PrimitiveCircle(IPrimitive* p,RenderSystem^ rendersys);

		void setCircle(Vector3F^ center,Vector3F^ normal,float radius);

	protected:
		Vector3F^ m_center;
		Vector3F^ m_normal;
		float m_radius;
	};

	//public ref class PrimitiveSphere : public Primitive
	//{
	//public:
	//	PrimitiveCircle(IPrimitive* p,RenderSystem^ rendersys);

	//	void setSphere();

	//protected:
	//};

	public ref class PrimitivePlane : public Primitive
	{
	public:
		PrimitivePlane(IPrimitive* p,RenderSystem^ rendersys);

		void setPlane(Vector3F^ center,Vector3F^ normal,float w,float h);

	protected:
		Vector3F^ m_center;
		Vector3F^ m_normal;
		float m_width;
		float m_height;
	};
}