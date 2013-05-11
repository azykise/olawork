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
	OlaTransformObj();
	virtual ~OlaTransformObj();

	inline void scene(OlaRenderScene* scene){mScene = scene;};
	inline OlaRenderScene* scene(){return mScene;};

	inline olaAABB* aabb(){return mAABB;};

	inline const olaVec3& position(){return mWorldPosition;};
	void position(const olaVec3& pos);
	void position(float x,float y,float z);

	inline const olaQuat& rotation(){return mWorldRotation;};
	void rotation(const olaQuat& rot);
	void rotation(float x,float y,float z,float w);

	inline const olaMat4& transform(){return mWorldTransform;};
	void transform(const olaMat4& trans);
	void transform(float* f44);

	inline OlaTransformObj* parent(){return mParent;};
	void parent(OlaTransformObj* par);

	inline unsigned int subCount(){return mSubs.size();};
	inline OlaTransformObj* subObj(unsigned int index){return mSubs[index];}
	void pushSubObj(OlaTransformObj* transobj);
	void popSubObj(OlaTransformObj* transobj);

	const olaVec3* localPosition(){return &mLocalPosition;};
	void localPosition(const olaVec3& lpos);
	void localPosition(float lx,float ly,float lz);

	const olaQuat* localRotation(){return &mLocalRotation;};
	void localRotation(const olaQuat& lrot);
	void localRotation(float lx,float ly,float lz,float lw);

	const olaMat4* localTransform(){return &mLocalTransform;}
	void localTransform(const olaMat4& ltrans);
	void localTransform(float* lf44); 

	inline TRANSFORM_OBJPARAM::OBJ_VISIABLE visiable(){return mVisiable;};
	inline void visiable(TRANSFORM_OBJPARAM::OBJ_VISIABLE vis){mVisiable = vis;};

	unsigned int kernelObjCount();
	OlaKernelObj* kernelObj(unsigned int index);
	void pushKernelObj(OlaKernelObj* obj);
	void popKernelObj(OlaKernelObj* obj);
	void clearKernelObjs();

protected:
	void _updateSubTransforms();
	void _updateWorldBasedLocal();
	void _updateLocalBasedWorld();
protected:
	OlaRenderScene* mScene;

	OlaTransformObj* mParent;
	OlaArray<OlaTransformObj*> mSubs;	

	olaQuat mWorldRotation;
	olaVec3 mWorldPosition;
	olaMat4 mWorldTransform;

	olaQuat mLocalRotation;
	olaVec3 mLocalPosition;
	olaMat4 mLocalTransform;

	olaAABB* mAABB;

	TRANSFORM_OBJPARAM::OBJ_VISIABLE mVisiable;

	OlaArray<OlaKernelObj*> mKernelObjs;
};

#endif