#ifndef _OLA_SHADER_RESOURCE_POOL_
#define _OLA_SHADER_RESOURCE_POOL_

#include "ola_util.h"

class OlaShader;
class OlaShaderPool
{
public:
	typedef OlaMap<olastring,OlaShader*> ShaderPool;
public:
	OlaShaderPool();

	virtual ~OlaShaderPool();

	void enPool(const char* fxassetpath,OlaShader* shader);
	void dePool(const char* fxassetpath);

	OlaShader* seek(const char* fxfilename);	

protected:
	ShaderPool mShaderPool;
};

#endif