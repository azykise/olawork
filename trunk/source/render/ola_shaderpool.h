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

	void enPool(OlaShader* shader);
	void dePool(OlaShader* shader);
	void dePool(const char* fxfilename);

	OlaShader* seek(const char* fxfilename);	

protected:
	ShaderPool mShaderPool;
};

#endif