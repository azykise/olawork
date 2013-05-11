#include "ola_light_impl.h"

#include "../ola_light.h"

#include "ola_transform_impl.h"

OlaLightImpl::OlaLightImpl( OlaLight* l):
mLight(l),
mTransform(0)
{
	mLight->addRef();
	mTransform = new OlaTransformImpl();
	mTransform->transform()->pushKernelObj(mLight);
}

OlaLightImpl::~OlaLightImpl()
{
	mTransform->transform()->popKernelObj(mLight);
	delete mTransform;
	mLight->delRef();
}

ola::ITransform* OlaLightImpl::transform()
{
	return mTransform;
}

void OlaLightImpl::setTargetPos( float x,float y,float z )
{	
	olaVec3 p(x,y,z);
	mLight->setLookAtPT(p);
}

void OlaLightImpl::getTargetPos( ola::vec3* out_pos )
{
	out_pos->x = mLight->lookatPT()->x;
	out_pos->y = mLight->lookatPT()->y;
	out_pos->z = mLight->lookatPT()->z;
}

