#ifndef _OLA_PRIMITIVE_H__
#define _OLA_PRIMITIVE_H__

#include "ola_mesh.h"

class OlaPrimitiveParam
{
public:
	enum PRIMITIVE_TYPE
	{
		PRI_LINE,
		PRI_PLANE,
	};
};

class OlaRenderOp;
class OlaRenderDevice;
class OlaPrimitive
{
public:
	OlaPrimitive(OlaPrimitiveParam::PRIMITIVE_TYPE type);

	virtual ~OlaPrimitive();

	virtual void setIndex(int* idxs,int num);

	virtual void setVertex(float* vec3s,int num);

	virtual void setColor(float r,float g,float b,float a);
	
	OlaRenderOp* renderop();

	inline unsigned long long id(){return mID;}

private:
	unsigned long long mID;
	OlaRenderOp* mOp;
	OlaVB* mVB;
	OlaIB* mIB;
	float mColor[4];
};

#endif
