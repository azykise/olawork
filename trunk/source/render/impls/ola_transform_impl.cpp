#include "ola_transform_impl.h"

#include "../win/win_impls.h"
#include "../ola_transobj.h"


OlaTransformImpl::OlaTransformImpl():
mTransform(0),
mSceneImpl(0)
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
	mSceneImpl = 0;
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

ola::IScene* OlaTransformImpl::scene()
{
	return mSceneImpl;
}

void OlaTransformImpl::detachScene()
{
	mSceneImpl->mScene->detachObj(mTransform);
	mSceneImpl = 0;
}
