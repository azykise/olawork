#include "ola_transobj.h"

#include "ola_scene.h"

OlaTransformObj::OlaTransformObj():
mScene(0),
mParent(0),
mVisiable(TRANSFORM_OBJPARAM::OBJVIS_NONE)
{
	mWorldTransform.Identity();
}

OlaTransformObj::~OlaTransformObj()
{
	if (mScene)
	{
		mScene->detachObj(this);
		mScene = 0;
	}
}

TRANSFORM_OBJPARAM::OBJ_VISIABLE OlaTransformObj::visiable()
{
	return mVisiable;
}

void OlaTransformObj::visiable( TRANSFORM_OBJPARAM::OBJ_VISIABLE vis )
{
	mVisiable = vis;
}

void OlaTransformObj::position( float x,float y,float z )
{
	olaVec3 v(x,y,z);
	position(v);
}

void OlaTransformObj::position( const olaVec3& pos )
{

}

const olaVec3* OlaTransformObj::position()
{
	return 0;
}

const olaQuat* OlaTransformObj::rotation()
{
	return 0;
}

void OlaTransformObj::rotation( const olaQuat& rot )
{

}

void OlaTransformObj::rotation( float x,float y,float z,float w )
{

}

const olaMat4* OlaTransformObj::transform()
{
	return 0;
}

void OlaTransformObj::transform( const olaMat4& trans )
{

}

void OlaTransformObj::transform( float* f44 )
{

}

OlaTransformObj* OlaTransformObj::parent()
{
	return mParent;
}

void OlaTransformObj::parent( OlaTransformObj* par )
{
	mParent = par;
}

OlaTransformObj::TransformList& OlaTransformObj::subs()
{
	return mSubs;
}

const olaVec3* OlaTransformObj::localPosition()
{
	return 0;
}

void OlaTransformObj::localPosition( const olaVec3& lpos )
{

}

void OlaTransformObj::localPosition( float lx,float ly,float lz )
{

}

const olaQuat* OlaTransformObj::localRotation()
{
	return 0;
}

void OlaTransformObj::localRotation( const olaQuat& lrot )
{

}

void OlaTransformObj::localRotation( float lx,float ly,float lz,float lw )
{

}

const olaMat4* OlaTransformObj::localTransform()
{
	return 0;
}

void OlaTransformObj::localTransform( const olaMat4& ltrans )
{

}

void OlaTransformObj::localTransform( float* lf44 )
{

}