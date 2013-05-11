#include "ola_transobj.h"

#include "ola_scene.h"
#include "ola_kernelobj.h"

#include "ola_bounding.h"

OlaTransformObj::OlaTransformObj():
mScene(0),
mParent(0),
mAABB(0),
mVisiable(TRANSFORM_OBJPARAM::OBJVIS_NONE)
{
	mWorldTransform.Identity();

	mAABB = new olaAABB();
}

OlaTransformObj::~OlaTransformObj()
{
	for (unsigned int i = 0 ; i < mKernelObjs.size() ; i++)
	{
		mKernelObjs[i]->delRef();
	}
	mKernelObjs.clear();

	if (mScene)
	{
		mScene->detachObj(this);
		mScene = 0;
	}

	if (mAABB)
	{
		delete mAABB;
		mAABB = 0;
	}
}

//----- override members
void OlaTransformObj::position( float x,float y,float z )
{
	mWorldPosition.Set(x,y,z);
	position(mWorldPosition);
}

void OlaTransformObj::rotation( float x,float y,float z,float w )
{
	mWorldRotation.Set(x,y,z,w);
	rotation(mWorldRotation);
}

void OlaTransformObj::localPosition( float lx,float ly,float lz )
{
	mLocalPosition.Set(lx,ly,lz);
	localPosition(mLocalPosition);
}

void OlaTransformObj::localRotation( float lx,float ly,float lz,float lw )
{
	mLocalRotation.Set(lx,ly,lz,lw);
	localRotation(mLocalRotation);
}

void OlaTransformObj::transform( float* f44 )
{
	mWorldTransform.FromFloatsColumnMajor(f44);

	transform(mWorldTransform);
}

void OlaTransformObj::localTransform( float* lf44 )
{
	mLocalTransform.FromFloatsColumnMajor(lf44);
	localTransform(mLocalTransform);
}

//----- override members

void OlaTransformObj::position( const olaVec3& pos )
{
	mWorldPosition = pos;
	mWorldTransform.SetTranslate(mWorldPosition);

	_updateLocalBasedWorld();

	_updateSubTransforms();
}

void OlaTransformObj::rotation( const olaQuat& rot )
{
	mWorldRotation = rot;
	mWorldTransform.SetRotation(mWorldRotation);

	_updateLocalBasedWorld();

	_updateSubTransforms();
}


void OlaTransformObj::transform( const olaMat4& trans )
{
	mWorldTransform = trans;

	mWorldTransform.GetTranslate(mWorldPosition);
	mWorldTransform.GetRotation(mWorldRotation);

	_updateLocalBasedWorld();

	_updateSubTransforms();
}

void OlaTransformObj::parent( OlaTransformObj* par )
{
	mParent = par;
	_updateLocalBasedWorld();
}

void OlaTransformObj::localPosition( const olaVec3& lpos )
{
	mLocalPosition = lpos;
	mLocalTransform.SetTranslate(mLocalPosition);
	mLocalTransform.SetTranslate(mWorldPosition);

	_updateWorldBasedLocal();

	_updateSubTransforms();
}

void OlaTransformObj::localRotation( const olaQuat& lrot )
{
	mLocalRotation = lrot;
	mLocalTransform.SetRotation(mLocalRotation);
	mWorldTransform.SetRotation(mWorldRotation);

	_updateWorldBasedLocal();

	_updateSubTransforms();
}

void OlaTransformObj::localTransform( const olaMat4& ltrans )
{
	mLocalTransform = ltrans;
	mLocalTransform.GetTranslate(mLocalPosition);
	mLocalTransform.GetRotation(mLocalRotation);

	_updateWorldBasedLocal();

	_updateSubTransforms();
}

unsigned int OlaTransformObj::kernelObjCount()
{
	return mKernelObjs.size();
}

OlaKernelObj* OlaTransformObj::kernelObj( unsigned int index )
{
	if (index >= mKernelObjs.size())
	{
		return 0;
	}
	return mKernelObjs[index];
}

void OlaTransformObj::pushKernelObj( OlaKernelObj* obj )
{
	if (!mKernelObjs.contains(obj))
	{
		obj->addRef();
		mKernelObjs.push_back(obj);
	}
}

void OlaTransformObj::popKernelObj( OlaKernelObj* obj )
{
	if (mKernelObjs.contains(obj))
	{
		obj->delRef();
		mKernelObjs.remove(obj);
	}
}

void OlaTransformObj::clearKernelObjs()
{
	for (unsigned int i = 0 ; i < mKernelObjs.size() ; i++)
	{
		mKernelObjs[i]->delRef();
	}
	mKernelObjs.clear();
}

void OlaTransformObj::pushSubObj( OlaTransformObj* transobj )
{
	if (!mSubs.contains(transobj))
	{
		transobj->parent(this);
		mSubs.push_back(transobj);
	}
}

void OlaTransformObj::popSubObj( OlaTransformObj* transobj )
{
	if (mSubs.contains(transobj))
	{
		transobj->parent(0);
		mSubs.remove(transobj);
	}
}

void OlaTransformObj::_updateSubTransforms()
{
	for (unsigned int i = 0 ; i < mSubs.size() ; i++)
	{
		OlaTransformObj* subt = mSubs[i];
		subt->_updateWorldBasedLocal();
		subt->_updateSubTransforms();
	}
}

void OlaTransformObj::_updateWorldBasedLocal()
{
	if (mParent)
	{
		mWorldTransform = mLocalTransform * mParent->transform();
		mWorldTransform.GetTranslate(mWorldPosition);
		mWorldTransform.GetRotation(mWorldRotation);
	}
	else
	{
		mWorldTransform = mParent->transform();
		mWorldTransform.GetTranslate(mWorldPosition);
		mWorldTransform.GetRotation(mWorldRotation);
	}
}

void OlaTransformObj::_updateLocalBasedWorld()
{
	if (mParent)
	{
		// mLocal * mWorld = mParentWorld
		// mLocal * mWorld * mWorldInv = mParentWorld * mWorldInv
		// mLocal = mParentWorld * mWorldInv
		olaMat4 worldInv = mWorldTransform.Inverse();
		mLocalTransform = mParent->transform() * worldInv;
		mLocalTransform.GetTranslate(mLocalPosition);
		mLocalTransform.GetRotation(mLocalRotation);
	}
	else
	{
		mLocalTransform = mWorldTransform;
		mLocalTransform.GetTranslate(mLocalPosition);
		mLocalTransform.GetRotation(mLocalRotation);
	}
}
