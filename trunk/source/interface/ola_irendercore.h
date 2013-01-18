#ifndef _OLA_INCLUDE_RNEDER_INTERFACE_H__
#define _OLA_INCLUDE_RNEDER_INTERFACE_H__

#include "ola_icommon.h"

namespace ola
{
	struct RenderWindowInfo
	{
		char name[128];
		unsigned int hwnd;
		int width;
		int height;
		bool full;
	};

	class IRenderCoreObj //由外部管理生命的对象
	{
	public:
		virtual ~IRenderCoreObj(){}
	};

	class ICamera : public IRenderCoreObj
	{
	public:
		virtual void setPrespect(float fovy,float aspect,float n,float f) = 0;
		
		virtual ITransform* transform() = 0;

		virtual void getViewMatrix(float* mat44) = 0;
		virtual void getProjMatrix(float* mat44) = 0;
	};

	class IRenderWindow : public IRenderCoreObj
	{
	public:	
		virtual void setBackgroundColor(float r,float g,float b,float a) = 0;

		virtual void setActive(bool a) = 0;
		virtual bool isActived() = 0;

		virtual void resize(int w,int h) = 0;

		virtual int width() = 0;
		virtual int height() = 0;
	};	

	enum PrimitiveType
	{
		PMT_UNKNOWN = -1,
		PMT_LINE,			//直线
	};

	class IPrimitive : public IRenderCoreObj
	{
	public:

		virtual unsigned long long getID() = 0;

		virtual PrimitiveType getType() = 0;

		//length 为 数组 vec3s 的长度
		virtual void setVertex(float* vec3s,int length) = 0;

		virtual void setColor(float r,float g,float b,float a) = 0;

		virtual void setIndex(int* idxs,int length) = 0;
	};

	typedef char byte;
	class IResource : public IRenderCoreObj
	{
	public:

		virtual int deserialize(const byte* data,int len) = 0;

	};

	class IGeometry;
	class IRenderable : public IRenderCoreObj
	{
	public:
		
		virtual void setActive(bool a) = 0;

		virtual bool isActive() = 0;

		virtual void setTransform(float* mat4_4) = 0;

		virtual void setGeometry(IGeometry* geo) = 0;

		virtual IGeometry* geometry() = 0;
	};

	class IShader : public IResource
	{
	public:
		virtual const char* name() = 0;
	};

	class IMaterial : public IResource
	{
	public:
		virtual const char* name() = 0;
	};

	class IGeometry : public IResource
	{
	public:
		virtual int submeshNum() = 0;

		virtual void setMaterial(int idx , IMaterial* mat) = 0;

		virtual IMaterial* material(int idx) = 0;

		virtual tAABB* aabb() = 0;

		virtual void setRootBone(ITransform* rb) = 0;
		virtual ITransform* rootbone() = 0;
	};

	class IIllumination : public IRenderCoreObj
	{
	public:
		enum ILLUMINATION_TYPE
		{
			ILLUMINATION_DIR = 0,
			ILLUMINATION_SPO,
			ILLUMINATION_POT,
		};
	public:
		
		virtual ITransform* transform() = 0;

		virtual void setTargetPos(float x,float y,float z) = 0;
		virtual void getTargetPos(tVec3* out_pos) = 0;
	};

	class IRenderCore
	{
	public:
		virtual ~IRenderCore(){};

		virtual bool initialize(unsigned int hwnd) = 0;
		virtual void release() = 0;

		virtual void beginDraw() = 0;
		virtual void drawScene() = 0;
		virtual void endDraw() = 0;

		virtual IRenderWindow* createWindow(RenderWindowInfo* info) = 0;

		virtual IGeometry* createGeometry() = 0;
		virtual IMaterial* createMaterial() = 0;
		virtual IShader* createShader() = 0;

		virtual IPrimitive*	createPrimitive(PrimitiveType type)= 0;
		virtual IIllumination* createIllumination() = 0;
		virtual IRenderable* createRenderable() = 0;

		virtual void releaseObject(IRenderCoreObj* object) = 0;

		virtual void cleanUnusedResources(const char* res_name) = 0;
		virtual int resourceNum(const char* res_name) = 0;
	};
}

#endif