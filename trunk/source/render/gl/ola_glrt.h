#ifndef _OLA_GL_RENDERTARGET_H__
#define _OLA_GL_RENDERTARGET_H__

#include "../ola_postprocess.h"

class OlaGLRenderTarget : public OlaRenderTarget
{
public:
	OlaGLRenderTarget(const char* name);

	virtual bool initialize( int w ,int h );

	virtual void release();

	virtual void* handle();

	int mHandles[OlaRTParam::RTPARAM_TYPE_END];	
};

class OlaGLOriginRT : public OlaGLRenderTarget
{
public:
	OlaGLOriginRT();
	~OlaGLOriginRT();

	virtual bool initialize(int w ,int h);	
	virtual void release();

	virtual void* handle()		{ return &mGLDefaultFB;}
protected:
	int mGLDefaultFB;
};

#endif