#ifndef __OLA__ENTITY__H__
#define __OLA__ENTITY__H__

#include "ola_string.h"
#include "math/ola_math.h"
#include "ola_bounding.h"

class OlaRenderScene;

struct TRANSFORM_OBJPARAM
{
	enum OBJ_FLAGS
	{
		OBJFLAG_LIGHT		= 1<<0,
		OBJFLAG_STATIC		= 1<<1,
		OBJFLAG_DYNAMIC		= 1<<2,
	};	
};

class OlaTransformObj
{
public:
	OlaTransformObj();
	virtual ~OlaTransformObj();

	virtual unsigned int flags() = 0;

	//should not be virtual cause it may be called in deconstrator
	inline void setScene(OlaRenderScene* scene){mScene = scene;};
	inline OlaRenderScene* scene(){return mScene;};

	virtual olaAABB* aabb() = 0;

	virtual const olaVec3* position() = 0;
	virtual void setPosition(const olaVec3& pos) = 0;
	virtual void setPosition(float x,float y,float z){olaVec3 v(x,y,z);setPosition(v);}

	//virtual const olaQuat* rotation() = 0;
	//virtual const olaMat4* transform() = 0;
	//virtual void setRotation(const olaQuat& rot) = 0;	

protected:
	OlaRenderScene* mScene;
};

#endif