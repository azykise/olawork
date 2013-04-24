#include <assert.h>

#include "ola_shaderpool.h"
#include "ola_device.h"
#include "ola_material.h"

OlaShaderPool::OlaShaderPool()
{

}

OlaShaderPool::~OlaShaderPool()
{
	assert(mShaderPool.size() == 0 && "Shader Pool is not clean!");
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

	//OlaShader* shader = new OlaShader();
	//OlaShaderFX* fx = GetRenderDevice()->spawnShaderFX();
	//fx->load(fxfilename);
	//shader->reset(fx);

	return 0;
}


void OlaShaderPool::enPool( OlaShader* shader )
{
	olastring _fxfilename(shader->name());
	ShaderPool::iterator i = mShaderPool.find(_fxfilename);
	if (i == mShaderPool.end())
	{
		shader->addRef();
		mShaderPool[_fxfilename] = shader;
	}
}

void OlaShaderPool::dePool( OlaShader* shader )
{
	dePool(shader->name());
}

void OlaShaderPool::dePool( const char* fxfilename )
{
	olastring _fxfilename(fxfilename);
	ShaderPool::iterator i = mShaderPool.find(_fxfilename);
	if (i != mShaderPool.end())
	{
		OlaShader* shader = i->second;
		shader->delRef();
		if (shader->refCount() == 0)
		{
			mShaderPool.erase(_fxfilename);
			delete shader;
		}
	}
}