#ifndef _OLA_INCLUDE_OLAGLSL_H__
#define _OLA_INCLUDE_OLAGLSL_H__

#include "../ola_shaderfx.h"

class OlaGLSL : public OlaShaderFX
{
public:
	OlaGLSL();
	virtual ~OlaGLSL();

	virtual bool load(const char* file_name , bool file = true);
	virtual unsigned int handleByName(const char* name,void* p0 = 0);
	virtual void* program(){return &mProgHandle;};
	virtual int requestHandle( OlaMaterialParam::SHADER_VARS type );

	void _clear();

	olastring mVertString;
	olastring mFragString;

	unsigned int mVertHandle;
	unsigned int mFragHandle;
	unsigned int mProgHandle;
};

#endif