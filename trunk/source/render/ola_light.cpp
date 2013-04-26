#include "ola_light.h"

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

void OlaLight::setPosition(const olaVec3& pos)
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

unsigned int OlaLight::flags()
{
	unsigned int f = 0;
	f |= TRANSFORM_OBJPARAM::OBJFLAG_LIGHT;
	f |= TRANSFORM_OBJPARAM::OBJFLAG_DYNAMIC;
	return f;
}