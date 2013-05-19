#include "ola_materialpool.h"
#include "ola_material.h"
#include "ola_resource.h"
#include "parser/ola_mat.h"

OlaMaterialPool::OlaMaterialPool(tResourcePools* ps):
mDefaultMaterial(0)
{
	OlaMATParser parser(ps);

	tMatFileInfo  matinfo;
	parser.parseMATFromData(DEFAULT_MATERIAL_DATA,strlen(DEFAULT_MATERIAL_DATA),&matinfo);

	mDefaultMaterial = new OlaMaterial("default.mat");
	parser.fillMAT(&matinfo,mDefaultMaterial);

	mDefaultMaterial->addRef();
}

OlaMaterialPool::~OlaMaterialPool()
{
	if (mDefaultMaterial)
	{
		delete mDefaultMaterial;
		mDefaultMaterial = 0;
	}
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
	if (!strcmp(matassetpath,DEFAULT_MATERIAL_NAME))
	{
		return mDefaultMaterial;
	}

	olastring _matassetpath(matassetpath);

	MaterialPool::iterator i = mMaterialPool.find(_matassetpath);
	if (i != mMaterialPool.end())
	{
		return i->second;
	}

	return 0;
}