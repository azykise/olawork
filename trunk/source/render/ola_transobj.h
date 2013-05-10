#ifndef __OLA__TRANSFORM_OBJECT__H__
#define __OLA__TRANSFORM_OBJECT__H__

#include "ola_util.h"
#include "math/ola_math.h"
#include "ola_bounding.h"

class OlaRenderScene;
class OlaKernelObj;

struct TRANSFORM_OBJPARAM
{
	enum OBJ_FLAGS
	{
		OBJFLAG_LIGHT		= 1<<0,
		OBJFLAG_STATIC		= 1<<1,
		OBJFLAG_DYNAMIC		= 1<<2,
	};	

	enum OBJ_VISIABLE
	{
		OBJVIS_NONE = 0,
		OBJVIS_ALL,
		OBJVIS_PART,
	};
};

//生命由需要transform的对象控制，随该对象销毁而销毁
//scene不控制transform对象的生命
class OlaTransformObj
{
public:
	typedef OlaArray<OlaTransformObj*> TransformList;
public:
	OlaTransformObj();
	virtual ~OlaTransformObj();

	virtual unsigned int flags() {return 0;};

	inline void scene(OlaRenderScene* scene){mScene = scene;};
	inline OlaRenderScene* scene(){return mScene;};

	virtual olaAABB* aabb(){return 0;};

	virtual const olaVec3* position();
	virtual void position(const olaVec3& pos);
	virtual void position(float x,float y,float z);

	virtual const olaQuat* rotation();
	virtual void rotation(const olaQuat& rot);
	virtual void rotation(float x,float y,float z,float w);

	virtual const olaMat4* transform();
	virtual void transform(const olaMat4& trans);
	virtual void transform(float* f44);

	virtual OlaTransformObj* parent();
	virtual void parent(OlaTransformObj* par);

	virtual TransformList& subs();

	virtual const olaVec3* localPosition();
	virtual void localPosition(const olaVec3& lpos);
	virtual void localPosition(float lx,float ly,float lz);

	virtual const olaQuat* localRotation();
	virtual void localRotation(const olaQuat& lrot);
	virtual void localRotation(float lx,float ly,float lz,float lw);

	virtual const olaMat4* localTransform();
	virtual void localTransform(const olaMat4& ltrans);
	virtual void localTransform(float* lf44); 

	virtual TRANSFORM_OBJPARAM::OBJ_VISIABLE visiable();
	virtual void visiable(TRANSFORM_OBJPARAM::OBJ_VISIABLE vis);

	virtual unsigned int kernelObjCount();
	virtual OlaKernelObj* kernelObj(unsigned int index);
	virtual void pushKernelObj(OlaKernelObj* obj);
	virtual void popKernelObj(OlaKernelObj* obj);
	virtual void clearKernelObjs();

protected:
	OlaRenderScene* mScene;

	OlaTransformObj* mParent;
	TransformList mSubs;	

	olaVec3 mWorldRotation;
	olaVec3 mWorldPosition;
	olaMat4 mWorldTransform;

	olaVec3 mLocalRotation;
	olaVec3 mLocalPosition;
	olaVec3 mLocalTransform;

	TRANSFORM_OBJPARAM::OBJ_VISIABLE mVisiable;

	OlaArray<OlaKernelObj*> mKernelObjs;
};

#endif