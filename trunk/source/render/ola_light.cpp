#include "ola_light.h"

#include "ola_render.h"

OlaLight::OlaLight(LIGHT_TYPE t):
mPosition(0,1000,0),
mLookAtPT(0,0,0),
mRadius(500.0f),
mLength(1000.0f),
mType(t)
{

}

void OlaLight::setLookAtPT(const olaVec3& lat)
{
	mLookAtPT = lat;
}

void OlaLight::position(const olaVec3& pos)
{
	mPosition = pos;
}

const olaVec3* OlaLight::lookatPT()
{
	return &mLookAtPT;
}

const olaVec3* OlaLight:: position()
{
	return &mPosition;
}

const char* OlaLight::kernelID()
{
	return "_LIGHT_";
}

OlaKernelObj::ENABLE_STATE OlaLight::enabled()
{
	return OlaKernelObj::ES_ENABLE_ALL;
}

void OlaLight::updateInternal( float elasped,OlaTransformObj* transObj )
{
	if (transObj)
	{
		mPosition = transObj->position();
	}
	else
	{
		mPosition.Set(0.0f,0.0f,0.0f);
	}
}

void OlaLight::renderInternal( OlaRender* r )
{
	r->pushLight(this);
}
