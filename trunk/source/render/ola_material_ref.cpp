#include <assert.h>
#include "ola_material_ref.h"

OlaMaterialReference::OlaMaterialReference( OlaMaterial* mat,OlaMaterialReferencePool* pool):
mMaterial(mat),
mPool(pool)
{
	if(mMaterial)
	{
		mMaterial->addRef();
	}
}

OlaMaterialReference::~OlaMaterialReference()
{
	if(mMaterial)
	{
		mMaterial->delRef();
		mMaterial = 0;
	}
}

void OlaMaterialReference::setMaterial( OlaMaterial* mat )
{
	if(mat == 0)
	{
		mMaterial->delRef();
		mMaterial = mat;
	}
	else if(mMaterial != mat)
	{
		mMaterial->delRef();
		mMaterial = mat;
		mMaterial->addRef();
	}
}

void OlaMaterialReference::delRef()
{
	IRefCounter::delRef();
	if (mRefCounter == 0)
	{
		this->_popOut();
		mPool->mUnusedQueue.push(this);
	}
}

void OlaMaterialReference::_setNext( OlaMaterialReference* ref )
{
	assert(ref->mNext == 0 && "OlaMaterialReference::_setNext next not 0");
	assert(ref->mPre == 0 && "OlaMaterialReference::_setNext pre not 0");

	OlaMaterialReference* ola_next = mNext;

	mNext = ref;

	ref->mPre = this;
	ref->mNext = ola_next;
}

void OlaMaterialReference::_popOut()
{
	if(mPre != 0 && mNext != 0)
	{
		mPre->mNext = mNext;
		mNext->mPre = mPre;
		mPre = 0;
		mNext = 0;
	}
	else if(mPre != 0 && mNext == 0)
	{
		mPre->mNext = mNext;
		mPre = 0;
		mNext = 0;
	}
	else if(mPre == 0 && mNext != 0)
	{
		mNext->mPre = 0;
		mPre = 0;
		mNext = 0;
	}
	else if(mPre == 0 && mNext == 0)
	{

	}
}

OlaMaterialReferencePool::OlaMaterialReferencePool():
mUsedRefHead(0)
{

}

OlaMaterialReferencePool::~OlaMaterialReferencePool()
{

}

OlaMaterialReference* OlaMaterialReferencePool::requireUnusedReference( OlaMaterial* mat )
{
	if(!mUsedRefHead)
	{
		mUsedRefHead = new OlaMaterialReference(mat,this);
		mUsedRefHead->mNext = 0;
		return mUsedRefHead;
	}
	else
	{
		if(mUnusedQueue.size() != 0)
		{
			OlaMaterialReference* ref = mUnusedQueue.front();
			mUnusedQueue.pop();

			ref->setMaterial(mat);
			
			mUsedRefHead->_setNext(ref);

			return ref;
		}
		else
		{
			OlaMaterialReference* ref = new OlaMaterialReference(mat,this);
			mUsedRefHead->_setNext(ref);
			return ref;
		}
	}
	return 0;
}

