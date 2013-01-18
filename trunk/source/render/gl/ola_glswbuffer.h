#ifndef _OLA_INCLUDE_OPENGL_SWAP_BUFFER_H__
#define _OLA_INCLUDE_OPENGL_SWAP_BUFFER_H__

#include "ola_gl.h"

class OlaEGLBufferWin : public OlaSwapBuffer
{
public:
	OlaEGLBufferWin(void* ph, OlaGLDevice* device);

	virtual ~OlaEGLBufferWin();

//	virtual void setScreenHandle( void* ph );

	virtual void resize( int x,int y );

	virtual void swapBuffer();

	virtual void* handle();

	void* mEGLContext;

protected:
	OlaGLDevice* mDevice;

	void* mEGLSurface;
	
	unsigned int mHWND;
};

#endif