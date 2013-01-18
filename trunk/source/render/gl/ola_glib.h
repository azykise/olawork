#ifndef _OLA_GL_INDEX_BUFFER_H__
#define _OLA_GL_INDEX_BUFFER_H__

#include "../ola_geobuffer.h"

class OlaGLIB : public OlaIB
{
public:
	OlaGLIB();

	virtual void unlock();

	virtual void release();

	virtual void* handle();

	unsigned int mHandle;
};

#endif