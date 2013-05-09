#include "ola_materialpool.h"
#include "ola_material.h"

OlaMaterialPool::OlaMaterialPool()
{

}

OlaMaterialPool::~OlaMaterialPool()
{

}

void OlaMaterialPool::enPool( const char* matassetpath , OlaMaterial* mat )
{
	olastring _matassetpath(matassetpath);
	MaterialPool::iterator i = mMaterialPool.find(_matassetpath);
	if (i == mMaterialPool.end())
	{
		mat->addRef();
		mMaterialPool[_matassetpath] = mat;
	}
}

void OlaMaterialPool::dePool( const char* matassetpath )
{
	olastring _matassetpath(matassetpath);
	MaterialPool::iterator i = mMaterialPool.find(_matassetpath);
	if (i != mMaterialPool.end())
	{
		OlaMaterial* mat = i->second;
		mat->delRef();
		if (mat->refCount() == 0)
		{
			mMaterialPool.erase(_matassetpath);
			delete mat;
		}
	}
}

OlaMaterial* OlaMaterialPool::seek( const char* matassetpath )
{
	olastring _matassetpath(matassetpath);

	MaterialPool::iterator i = mMaterialPool.find(_matassetpath);
	if (i != mMaterialPool.end())
	{
		i->second->addRef();
		return i->second;
	}

	return 0;
}