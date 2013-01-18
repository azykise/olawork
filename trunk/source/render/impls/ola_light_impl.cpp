#include "ola_light_impl.h"

#include "../ola_light.h"

OlaLightImpl::OlaLightImpl( OlaLight* l):
mLight(l)
{

}

OlaLightImpl::~OlaLightImpl()
{
	delete mLight;
}

void OlaLightImpl::setPosition( float x,float y,float z )
{
	olaVec3 p(x,y,z);
	mLight->setPosition(p);
}

void OlaLightImpl::getPosition( ola::tVec3* out_pos )
{
	out_pos->x = mLight->position()->x;
	out_pos->y = mLight->position()->y;
	out_pos->z = mLight->position()->z;
}

void OlaLightImpl::getTargetPos( ola::tVec3* out_pos )
{
	out_pos->x = mLight->lookatPT()->x;
	out_pos->y = mLight->lookatPT()->y;
	out_pos->z = mLight->lookatPT()->z;
}

void OlaLightImpl::setTargetPos( float x,float y,float z )
{
	olaVec3 p(x,y,z);
	mLight->setLookAtPT(p);
}

