#ifndef _OLA_INCLUDE_SHADERFX_H__
#define _OLA_INCLUDE_SHADERFX_H__

#include "ola_string.h"
#include "ola_resource.h"
#include "ola_material.h"

//shader program file : glsl or hlsl or cg
class OlaShaderFX
{
public:
	OlaShaderFX(){};
	virtual ~OlaShaderFX(){};

	virtual bool load(const char* file_name) = 0;	
	virtual unsigned int handleByName(const char* name,void* p0 = 0) = 0;
	virtual void* program() = 0;

	virtual int requestHandle(OlaMaterialParam::SHADER_VARS type) = 0;

	virtual olastring& filename() {return mFilename;}
protected:
	olastring mFilename;
};

#endif