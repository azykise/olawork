#ifndef _OLA_GL_VERTEX_BUFFER_H__
#define _OLA_GL_VERTEX_BUFFER_H__

#include "../ola_geobuffer.h"

class OlaGLVB : public OlaVB
{
public:
	OlaGLVB();

	virtual void unlock();

	virtual void release();

	virtual void* handle();

	unsigned int mHandle;
};

#endif