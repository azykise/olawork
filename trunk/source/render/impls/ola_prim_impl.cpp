#include "ola_prim_impl.h"

#include "../ola_primitive.h"


OlaPrimitiveImpl::OlaPrimitiveImpl( OlaPrimitive* prim ):
mPrimitive(prim)
{
}

OlaPrimitiveImpl::~OlaPrimitiveImpl()
{
	if (mPrimitive)
	{
		delete mPrimitive;
		mPrimitive = 0;
	}
}

unsigned long long OlaPrimitiveImpl::getID()
{
	return mPrimitive->id();
}

void OlaPrimitiveImpl::setVertex( float* vec3s,int length )
{
	mPrimitive->setVertex(vec3s,length);
}

void OlaPrimitiveImpl::setColor( float r,float g,float b,float a )
{
	mPrimitive->setColor(r,g,b,a);
}

void OlaPrimitiveImpl::setIndex( int* idxs,int length )
{
	mPrimitive->setIndex(idxs,length);
}
