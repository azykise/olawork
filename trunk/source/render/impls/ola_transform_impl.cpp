#include "ola_transform_impl.h"

#include "../ola_transobj.h"


OlaTransformImpl::OlaTransformImpl():
mTransform(0)
{
	mTransform = new OlaTransformObj();
}

OlaTransformImpl::~OlaTransformImpl()
{
	if (mTransform)
	{		
		delete mTransform;
		mTransform = 0;
	}
}

OlaTransformObj* OlaTransformImpl::transform()
{
	return mTransform;
}

void OlaTransformImpl::getPosition( ola::vec3* out_pos )
{

}

void OlaTransformImpl::setPosition( float x,float y,float z )
{

}

void OlaTransformImpl::getRotation( ola::quat* out_rot )
{

}

void OlaTransformImpl::setRotation( float axisx,float axisy,float axisz,float degree )
{

}
