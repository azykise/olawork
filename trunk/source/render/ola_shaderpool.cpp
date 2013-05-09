#include <assert.h>

#include "ola_shaderpool.h"
#include "ola_device.h"
#include "ola_material.h"

OlaShaderPool::OlaShaderPool()
{

}

OlaShaderPool::~OlaShaderPool()
{
	//assert(mShaderPool.size() == 0 && "Shader Pool is not clean!");
}

OlaShader* OlaShaderPool::seek( const char* fxfilename )
{
	olastring _fxfilename(fxfilename);

	ShaderPool::iterator i = mShaderPool.find(_fxfilename);
	if (i != mShaderPool.end())
	{
		i->second->addRef();
		return i->second;
	}

	return 0;
}

void OlaShaderPool::enPool( const char* fxassetpath, OlaShader* shader )
{
	olastring _fxassetpath(fxassetpath);
	ShaderPool::iterator i = mShaderPool.find(_fxassetpath);
	if (i == mShaderPool.end())
	{
		shader->addRef();
		mShaderPool[_fxassetpath] = shader;
	}
}

void OlaShaderPool::dePool( const char* fxassetpath )
{
	olastring _fxassetpath(fxassetpath);
	ShaderPool::iterator i = mShaderPool.find(_fxassetpath);
	if (i != mShaderPool.end())
	{
		OlaShader* shader = i->second;
		shader->delRef();
		if (shader->refCount() == 0)
		{
			mShaderPool.erase(_fxassetpath);
			delete shader;
		}
	}
}