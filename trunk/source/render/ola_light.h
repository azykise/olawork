#ifndef _OLA_INCLUDE_OLA_LIGHT_H__
#define _OLA_INCLUDE_OLA_LIGHT_H__

#include "ola_util.h"

#include "math/ola_math.h"

#include "ola_kernelobj.h"
#include "ola_transobj.h"

class OlaLight : public OlaKernelObj
{
public:
	enum LIGHT_TYPE
	{
		TYPE_DIR = 0,
		TYPE_SPOT,
		TYPE_POINT,
		TYPE_END
	};
public:
	OlaLight(LIGHT_TYPE t = TYPE_DIR);

	virtual void setLookAtPT(const olaVec3& dir);
	virtual void position(const olaVec3& pos);

	void setLength(float l){mLength = l;}
	void setRadius(float r){mRadius = r;}

	float radius(){return mRadius;}
	float length(){return mLength;}

	virtual const olaVec3* lookatPT();
	virtual const olaVec3* position();

	virtual olaAABB* aabb(){return 0;}

	virtual const char* kernelID();

	virtual ENABLE_STATE enabled();

	virtual void updateInternal( float elasped,OlaTransformObj* transObj );

	virtual void renderInternal( OlaRender* r );


protected:
	
	olaVec3 mLookAtPT;
	olaVec3 mPosition;

	float mRadius;
	float mLength;

	LIGHT_TYPE mType;
};

#endif