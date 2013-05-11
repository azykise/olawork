#include <assert.h>

#include "ola_shaderpool.h"
#include "ola_device.h"
#include "ola_material.h"
#include "ola_resource.h"

#include "ola_shaderfx.h"

OlaShaderPool::OlaShaderPool()
{
	mDefaultShader = new OlaShader();
	OlaShaderFX* fx = GetRenderDevice()->spawnShaderFX();
	fx->load(DEFAULT_SHADER_DATA,false);
	mDefaultShader->reset(fx);
}

OlaShaderPool::~OlaShaderPool()
{
	if (mDefaultShader)
	{
		delete mDefaultShader;
		mDefaultShader = 0;
	}
	//assert(mShaderPool.size() == 0 && "Shader Pool is not clean!");
}

OlaShader* OlaShaderPool::seek( const char* fxfilename )
{
	if (!strcmp(fxfilename,DEFAULT_SHADER_NAME))
	{
		return mDefaultShader;
	}
	olastring _fxfilename(fxfilename);

	ShaderPool::iterator i = mShaderPool.find(_fxfilename);
	if (i != mShaderPool.end())
	{
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

OlaShader* OlaShaderPool::defshader()
{
	return mDefaultShader;
}
