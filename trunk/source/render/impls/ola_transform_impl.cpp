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
	out_pos->x = mTransform->position().x;
	out_pos->y = mTransform->position().y;
	out_pos->z = mTransform->position().z;

}

void OlaTransformImpl::setPosition( float x,float y,float z )
{
	mTransform->position(x,y,z);
}

void OlaTransformImpl::getRotation( ola::quat* out_rot )
{
	out_rot->x = mTransform->rotation().x;
	out_rot->y = mTransform->rotation().y;
	out_rot->z = mTransform->rotation().z;
	out_rot->w = mTransform->rotation().w;
}

void OlaTransformImpl::setRotation( float x,float y,float z,float w )
{
	mTransform->rotation(x,y,z,w);
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
