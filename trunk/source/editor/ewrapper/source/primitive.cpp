#include "primitive.h"
#include "render_system.h"
#include "tool.h"

namespace Engine
{
	using namespace Tool;
	ref struct tLine 
	{
		tLine(Vector3F^ _start,Vector3F^ _end)
		{
			start = _start;
			end = _end;
		}

		System::Collections::Generic::List<float>^ toList()
		{			
			System::Collections::Generic::List<float>^ f = gcnew System::Collections::Generic::List<float>();
			f->Add(start->X); f->Add(start->Y); f->Add(start->Z);
			f->Add(end->X); f->Add(end->Y); f->Add(end->Z);
			return f;
		}
		
		Vector3F^ start;
		Vector3F^ end;
	};

	Primitive::Primitive(IPrimitive* p,RenderSystem^ rendersys):
	m_primitive(p),
	m_rendersys(rendersys)
	{
		m_id = m_primitive->getID();
	}

	Primitive::~Primitive()
	{
		if(m_primitive)
		{
			m_rendersys->RenderCore->releaseObject(m_primitive);
			m_primitive = 0;
		}
	}

	Primitive::!Primitive()
	{
		if(m_primitive)
		{
			m_rendersys->RenderCore->releaseObject(m_primitive);
			m_primitive = 0;
		}
	}

	void Primitive::setColor(float r,float g,float b,float a)
	{
		m_primitive->setColor(r,g,b,a);
	}

	void Primitive::render(RenderSystem^ rendersys)
	{
		rendersys->pushToRender(this);
	}

	float Primitive::computeDottedSpace(float line_length)
	{
		int i = (int)(line_length * 10.0f);
		return 0;
	}

	PrimitiveLine::PrimitiveLine(IPrimitive* p,RenderSystem^ rendersys):
	Primitive(p,rendersys)
	{
		m_data = gcnew array<float>(6);
	}

	void PrimitiveLine::setLine(Vector3F^ start,Vector3F^ end)
	{
		m_data[0] = start->X ; m_data[1] = start->Y ; m_data[2] = start->Z;
		m_data[3] = end->X ; m_data[4] = end->Y ; m_data[5] = end->Z;

		float p[6];
		arrayToUnSafe(m_data,p);

		int idx[] = {0,1};

		m_primitive->setVertex(p,6);
		m_primitive->setIndex(idx,2);
	}

	void PrimitiveLine::setLine(float x0,float y0,float z0,float x1,float y1,float z1)
	{
		m_data[0] = x0 ; m_data[1] = y0 ; m_data[2] = z0;
		m_data[3] = x1 ; m_data[4] = y1 ; m_data[5] = z1;

		float p[6];
		arrayToUnSafe(m_data,p);

		int idx[] = {0,1};

		m_primitive->setVertex(p,6);
		m_primitive->setIndex(idx,2);
	}


	PrimitiveGrid::PrimitiveGrid(IPrimitive* p,RenderSystem^ rendersys):
	Primitive(p,rendersys)
	{
		m_center = gcnew Vector3F(0,0,0);
		m_grid_hnum = m_grid_vnum = 1;
		m_space_h = m_space_v = 1;
		m_rectpt = new float[24];
	}

	PrimitiveGrid::~PrimitiveGrid()
	{
		delete[] m_rectpt;
	}

	void PrimitiveGrid::setGrid(Vector3F^ center,int grid_hnum,int grid_vnum,int space_h,int space_v)
	{
		m_center = gcnew Vector3F(center->X,center->Y,center->Z);
		m_grid_hnum = grid_hnum;
		m_grid_vnum = grid_vnum;
		m_space_h = (float)space_h;
		m_space_v = (float)space_v;

		int line_hnum = grid_hnum + 1;
		int line_vnum = grid_vnum + 1;

		float x_start = -1.0f * ((float)m_grid_hnum) / 2.0f  * m_space_h;
		float x_end = ((float)m_grid_hnum) / 2.0f * m_space_h;
		float y_start = -1.0f * ((float)m_grid_vnum) / 2.0f  * m_space_v;
		float y_end = ((float)m_grid_vnum) / 2.0f * m_space_v;

		if(m_data == nullptr)
			m_data = gcnew array<float>(6 * (line_vnum + line_hnum));
		else if(m_data->Length != 6 * (line_vnum + line_hnum))
			m_data = gcnew array<float>(6 * (line_vnum + line_hnum));

		for (int i = 0; i < line_hnum ; i++)
		{		
			m_data[i * 6 + 0] = x_start; 
			m_data[i * 6 + 1] = y_start + i * m_space_v; 
			m_data[i * 6 + 2] = m_center->Z;
			m_data[i * 6 + 3] = x_end;   
			m_data[i * 6 + 4] = y_start + i * m_space_v; 
			m_data[i * 6 + 5] = m_center->Z;
		}

		for (int i = line_hnum ; i < line_hnum + line_vnum ; i++)
		{
			m_data[i * 6 + 0] = x_start + (i - line_hnum) * m_space_h; 
			m_data[i * 6 + 1] = y_start; 
			m_data[i * 6 + 2] = m_center->Z;
			m_data[i * 6 + 3] = x_start + (i - line_hnum) * m_space_h;   
			m_data[i * 6 + 4] = y_end; 
			m_data[i * 6 + 5] = m_center->Z;
		}

		float* f = new float[6 * (line_vnum + line_hnum)];
		arrayToUnSafe(m_data,f);
		m_primitive->setVertex(f,6 * (line_vnum + line_hnum));

		int idx_num = 2 * (line_hnum + line_vnum);
		int* idx = new int[idx_num];
		for (int i = 0 ; i < idx_num; i++)
		{
			idx[i] = i;
		}
		m_primitive->setIndex(idx,idx_num);

		delete[] f;
		delete[] idx;
	}

	void PrimitiveGrid::setRect(Vector3F^ v0,Vector3F^ v1,Vector3F^ v2,Vector3F^ v3)
	{
		m_center = (*v0 + *v1 + *v2 + *v3)/4.0f;
		m_grid_hnum = m_grid_vnum = 1;
		m_space_h = (*v1 - *v0).GetLength();
		m_space_v = (*v3 - *v0).GetLength();

		if(m_data == nullptr)
			m_data = gcnew array<float>(24);
		else if(m_data->Length != 24)
			m_data = gcnew array<float>(24);

		int line = 0;
		m_data[line * 6 + 0] = v0->X; m_data[line * 6 + 1] = v0->Y ; m_data[line * 6 + 2] = v0->Z;
		m_data[line * 6 + 3] = v1->X; m_data[line * 6 + 4] = v1->Y ; m_data[line * 6 + 5] = v1->Z; line += 1;

		m_data[line * 6 + 0] = v1->X; m_data[line * 6 + 1] = v1->Y ; m_data[line * 6 + 2] = v1->Z;
		m_data[line * 6 + 3] = v2->X; m_data[line * 6 + 4] = v2->Y ; m_data[line * 6 + 5] = v2->Z; line += 1;

		m_data[line * 6 + 0] = v2->X; m_data[line * 6 + 1] = v2->Y ; m_data[line * 6 + 2] = v2->Z;
		m_data[line * 6 + 3] = v3->X; m_data[line * 6 + 4] = v3->Y ; m_data[line * 6 + 5] = v3->Z; line += 1;

		m_data[line * 6 + 0] = v3->X; m_data[line * 6 + 1] = v3->Y ; m_data[line * 6 + 2] = v3->Z;
		m_data[line * 6 + 3] = v0->X; m_data[line * 6 + 4] = v0->Y ; m_data[line * 6 + 5] = v0->Z; line += 1;

		arrayToUnSafe(m_data,m_rectpt);
		m_primitive->setVertex(m_rectpt,24);

		int index[] = {0,1,2,3,4,5,6,7};
		m_primitive->setIndex(index,8);
	}

	PrimitiveBorderCube::PrimitiveBorderCube(IPrimitive* p,RenderSystem^ rendersys):
	Primitive(p,rendersys)
	{

	}

	void PrimitiveBorderCube::setAsAABBCube(Vector3F^ min,Vector3F^ max)
	{
		m_center = gcnew Vector3F((min->X + max->X)/2.0f,
								  (min->Y + max->Y)/2.0f,
								  (min->Z + max->Z)/2.0f);

		setBorderCube(m_center,
			System::Math::Abs(min->X - max->X),
			System::Math::Abs(min->Y - max->Y),
			System::Math::Abs(min->Z - max->Z));
	}

	void PrimitiveBorderCube::setBorderCube(Vector3F^ center,float w,float h,float l)
	{
		m_center = gcnew Vector3F(center->X,center->Y,center->Z);
		m_length = l;
		m_width = w;
		m_height = h;
		
		float half_l = m_length / 2.0f;
		float quta_l = m_length / 4.0f;

		float half_w = m_width / 2.0f;
		float quta_w = m_width / 4.0f;

		float half_h = m_height / 2.0f;
		float quta_h = m_height / 4.0f;

		float corners[8][3]; //数组方式的实现
		corners[0][0] = center->X - half_w; corners[0][1] = center->Y + half_h; corners[0][2] = center->Z + half_l;
		corners[1][0] = center->X + half_w; corners[1][1] = center->Y + half_h; corners[1][2] = center->Z + half_l;
		corners[2][0] = center->X + half_w; corners[2][1] = center->Y - half_h; corners[2][2] = center->Z + half_l;
		corners[3][0] = center->X - half_w; corners[3][1] = center->Y - half_h; corners[3][2] = center->Z + half_l;
		corners[4][0] = center->X - half_w; corners[4][1] = center->Y + half_h; corners[4][2] = center->Z - half_l;
		corners[5][0] = center->X + half_w; corners[5][1] = center->Y + half_h; corners[5][2] = center->Z - half_l;
		corners[6][0] = center->X + half_w; corners[6][1] = center->Y - half_h; corners[6][2] = center->Z - half_l;
		corners[7][0] = center->X - half_w; corners[7][1] = center->Y - half_h; corners[7][2] = center->Z - half_l;

		int vertex_num = 48*3;

		if(m_data == nullptr)
			m_data = gcnew array<float>(vertex_num);
		else if(m_data->Length != vertex_num)
			m_data = gcnew array<float>(vertex_num);

		int c_idx = 0; int p_idx = 0;

		//corners[0]
		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 
		m_data[p_idx + 0] = corners[c_idx][0] + quta_w	; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 

		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 
		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1] - quta_h	; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3;

		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 
		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]  - quta_l	; p_idx += 3; c_idx += 1;

		//corners[1]
		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 
		m_data[p_idx + 0] = corners[c_idx][0] - quta_w	; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 

		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 
		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1] - quta_h	; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3;

		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 
		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]  - quta_l	; p_idx += 3; c_idx += 1;

		//corners[2]
		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 
		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1] + quta_h	; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 

		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 
		m_data[p_idx + 0] = corners[c_idx][0] - quta_w	; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3;

		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 
		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]  - quta_l	; p_idx += 3; c_idx += 1;

		//corners[3]
		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 
		m_data[p_idx + 0] = corners[c_idx][0] + quta_w	; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 

		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 
		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1] + quta_h	; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3;

		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 
		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]  - quta_l	; p_idx += 3; c_idx += 1;

		//corners[4]
		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 
		m_data[p_idx + 0] = corners[c_idx][0] + quta_w	; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 

		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 
		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1] - quta_h	; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3;

		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 
		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]  + quta_l	; p_idx += 3; c_idx += 1;

		//corners[5]
		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 
		m_data[p_idx + 0] = corners[c_idx][0] - quta_w	; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 

		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 
		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1] - quta_h	; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3;

		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 
		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]  + quta_l	; p_idx += 3; c_idx += 1;

		//corners[6]
		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 
		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1] + quta_h	; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 

		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 
		m_data[p_idx + 0] = corners[c_idx][0] - quta_w	; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3;

		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 
		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]  + quta_l	; p_idx += 3; c_idx += 1;

		//corners[7]
		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 
		m_data[p_idx + 0] = corners[c_idx][0] + quta_w	; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 

		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 
		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1] + quta_h	; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3;

		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]				; p_idx += 3; 
		m_data[p_idx + 0] = corners[c_idx][0]			; m_data[p_idx + 1] = corners[c_idx][1]				; m_data[p_idx + 2] = corners[c_idx][2]  + quta_l	; p_idx += 3; c_idx += 1;

		float* f = new float[vertex_num];
		arrayToUnSafe(m_data,f);
		m_primitive->setVertex(f,vertex_num);
		delete[] f;

		int idx_num = vertex_num / 3;
		int* index = new int[idx_num];
		for (int i = 0 ; i < idx_num ; i++)
		{
			index[i] = i;
		}
		m_primitive->setIndex(index,idx_num);
		delete[] index;
	}

	static const int CBUE_VERTEX_NUM = 8;

	PrimitiveCube::PrimitiveCube(IPrimitive* p,RenderSystem^ rendersys):
	Primitive(p,rendersys)
	{
		m_center = gcnew Vector3F(0,0,0);
		m_size = 1.0f;
		m_data = gcnew array<float>(CBUE_VERTEX_NUM * 3);
	}

	void PrimitiveCube::setCube(Vector3F^ center,float size)
	{

	}

	static const int CIRCLE_VERTEX_NUM = 360;

	PrimitiveCircle::PrimitiveCircle(IPrimitive* p,RenderSystem^ rendersys):
	Primitive(p,rendersys)
	{
		m_center = gcnew Vector3F(0.0f,0.0f,0.0f);
		m_normal = gcnew Vector3F(0.0f,0.0f,0.0f);
		m_data = gcnew array<float>(CIRCLE_VERTEX_NUM * 3);
	}

	void PrimitiveCircle::setCircle(Vector3F^ center,Vector3F^ normal,float radius)
	{
		m_center->X = center->X;m_center->Y = center->Y;m_center->Z = center->Z;
		m_normal->X = normal->X;m_normal->Y = normal->Y;m_normal->Z = normal->Z;
		m_radius = radius;

		m_normal->Normalize();

		Vector3F u(0.0f,0.0f,0.0f);

		if(Math::IsZero(m_normal->X) && Math::IsZero(m_normal->Y))
			u.X = 1.0f;
		else if(Math::IsZero(m_normal->X) && Math::IsZero(m_normal->Z))
			u.Z = 1.0f;
		else if(Math::IsZero(m_normal->Y) && Math::IsZero(m_normal->Z))
			u.Y = 1.0f;
		else 
			u = Vector3F(m_normal->Y,-1.0f * m_normal->X,0.0f);

		Vector3F v = Vector3F::CrossProduct(*m_normal,u);

		u.Normalize();
		v.Normalize();

		float rad = 1.0f/(2.0f * (float)System::Math::PI);
		float angle = 0.0f;
		for (int i = 0 ; i < CIRCLE_VERTEX_NUM ; i++)
		{
			m_data[i * 3 + 0] = m_center->X + m_radius * (u.X * (float)System::Math::Cos(angle) + v.X * (float)System::Math::Sin(angle));
			m_data[i * 3 + 1] = m_center->Y + m_radius * (u.Y * (float)System::Math::Cos(angle) + v.Y * (float)System::Math::Sin(angle));
			m_data[i * 3 + 2] = m_center->Z + m_radius * (u.Z * (float)System::Math::Cos(angle) + v.Z * (float)System::Math::Sin(angle));
			angle += rad;
		}

		float* f = new float[CIRCLE_VERTEX_NUM * 3];
		arrayToUnSafe(m_data,f);
		m_primitive->setVertex(f,CIRCLE_VERTEX_NUM * 3);
		delete[] f;		
	}

	PrimitivePlane::PrimitivePlane(IPrimitive* p,RenderSystem^ rendersys):
	Primitive(p,rendersys)
	{
		m_center = gcnew Vector3F(0.0f,0.0f,0.0f);
		m_normal = gcnew Vector3F(0.0f,0.0f,0.0f);
		m_width = 1.0f;
		m_height = 1.0f;
		m_data = gcnew array<float>(4 * 3);
	}

	void PrimitivePlane::setPlane(Vector3F^ center,Vector3F^ normal,float w,float h)
	{
		m_center->X = center->X;m_center->Y = center->Y;m_center->Z = center->Z;
		m_normal->X = normal->X;m_normal->Y = normal->Y;m_normal->Z = normal->Z;
		m_width  = w;
		m_height = h;

		if(Math::IsZero(m_normal->X) && Math::IsZero(m_normal->Y) && Math::IsZero(m_normal->Z))
		{
			m_normal->X = 0;m_normal->Y = 0;m_normal->Z = 1;
		}
		else
			m_normal->Normalize();

		Vector4F v0,v1,v2,v3;
		Vector3F^ org = gcnew Vector3F(0,0,0);
		v0.X = org->X - w/2; v0.Y = org->Y + h/2; v0.Z = org->Z +0; v0.W = 1.0f;
		v1.X = org->X + w/2; v1.Y = org->Y + h/2; v1.Z = org->Z +0; v1.W = 1.0f;
		v2.X = org->X + w/2; v2.Y = org->Y - h/2; v2.Z = org->Z +0; v2.W = 1.0f;
		v3.X = org->X - w/2; v3.Y = org->Y - h/2; v3.Z = org->Z +0; v3.W = 1.0f;

		Vector3F right = Vector3F::ZAxis;
     	if(Math::IsZero(m_normal->X) && Math::IsZero(m_normal->Y) && !Math::IsZero(m_normal->Z))
			right = Vector3F::XAxis;
		Vector3F axis = Vector3F::CrossProduct(right,*m_normal);
		axis.Normalize();		
		float cos = Vector3F::DotProduct(Vector3F::ZAxis,*m_normal);
		float angle = static_cast<float>(System::Math::Acos(cos));

		if(!Math::IsZero(angle))
		{					
			QuaternionF q = QuaternionF::FromAxisAngle(axis,angle);
			q.Normalize();

			Matrix4F m = QuaternionF::QuaternionToMatrix(q);				

			Vector4F v(m_center->X,m_center->Y,m_center->Z,0);

			v0 = v + Matrix4F::Transform(m,v0);
			v1 = v + Matrix4F::Transform(m,v1);
			v2 = v + Matrix4F::Transform(m,v2);
			v3 = v + Matrix4F::Transform(m,v3);
		}

		v0.X += m_center->X; v0.Y += m_center->Y; v0.Z += m_center->Z;
		v1.X += m_center->X; v1.Y += m_center->Y; v1.Z += m_center->Z;
		v2.X += m_center->X; v2.Y += m_center->Y; v2.Z += m_center->Z;
		v3.X += m_center->X; v3.Y += m_center->Y; v3.Z += m_center->Z;

		m_data[0] = v0.X;
		m_data[1] = v0.Y;
		m_data[2] = v0.Z;

		m_data[3] = v1.X;
		m_data[4] = v1.Y;
		m_data[5] = v1.Z;

		m_data[6] = v2.X;
		m_data[7] = v2.Y;
		m_data[8] = v2.Z;

		m_data[9]  = v3.X;
		m_data[10] = v3.Y;
		m_data[11] = v3.Z;

		int index[] = {0,3,2,2,1,0};
		m_primitive->setIndex(index,6);

		float* f = new float[4 * 3];
		arrayToUnSafe(m_data,f);
		m_primitive->setVertex(f,4 * 3);
		delete[] f;	
	}

}