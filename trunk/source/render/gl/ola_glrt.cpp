#include <stdio.h>
#include <assert.h>

#if defined WIN32
#include <GLES2/gl2.h>
#elif defined __ANDROID_API__
#include <GLES2/gl2.h>
#elif defined __APPLE__
#include <OpenGLES/ES2/gl.h>
#endif

#define lgGLError(x) 	if(glGetError() != GL_NO_ERROR){lg(x); }

#include "ola_glrt.h"

bool OlaGLRenderTarget::initialize( int w ,int h )
{
	release();

	mWidth = w;
	mHeight = h;

//color buffer
	unsigned int color_rb = 0;
	glGenTextures(1, &color_rb);
	lgGLError("color buffer gen failed in rt \n");

    glBindTexture(GL_TEXTURE_2D, color_rb);
	lgGLError("color buffer bind failed in rt \n");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE , 0); //rgba  GL_UNSIGNED_SHORT_5_6_5 GL_UNSIGNED_BYTE
	lgGLError("color buffer create failed in rt \n");

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_2D, 0);

//depth buffer
	unsigned int depth_rb = 0;
	glGenTextures(1, &depth_rb);
	lgGLError("depth buffer gen failed in rt \n");

    glBindTexture(GL_TEXTURE_2D, depth_rb);
	lgGLError("depth buffer bind failed in rt \n");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, mWidth, mHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT , 0); //rgba  GL_UNSIGNED_SHORT_5_6_5 GL_UNSIGNED_BYTE
	lgGLError("depth buffer create failed in rt \n");

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_2D, 0);

//stencil buffer
	//unsigned int stencel_buffer = 0;
	//glGenRenderbuffers(1,&stencel_buffer);
	//glBindRenderbuffer(GL_RENDERBUFFER,stencel_buffer);
	//glRenderbufferStorage(GL_RENDERBUFFER,GL_STENCIL_INDEX8,mWidth,mHeight);
	//lgGLError("stencil buffer create failed in rt \n");
	//glBindRenderbuffer(GL_RENDERBUFFER,0);


	unsigned int fbo = 0;
	glGenFramebuffers(1,&fbo);
	glBindFramebuffer(GL_FRAMEBUFFER,fbo);
	lgGLError("frame buffer gen failed in rt \n");
	mHandles[OlaRTParam::FRAME_BUFFER] = fbo;

	mHandles[OlaRTParam::COLOR_BUFFER] = color_rb;
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D, color_rb,0);
	lgGLError("color buffer attach failed in rt \n");

	mHandles[OlaRTParam::DEPTH_BUFFER] = depth_rb;
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D, depth_rb, 0);
	lgGLError("depth buffer attach failed in rt \n");

	//mHandles[OlaRTParam::STENCIL_BUFFER] = stencel_buffer;
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_STENCIL_ATTACHMENT,GL_RENDERBUFFER,stencel_buffer); 
	//lgGLError("stencil buffer attach failed in rt \n");

	GLenum hr = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch (hr)
	{
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		lg("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
		lg("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		lg("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
		break;
	case GL_FRAMEBUFFER_UNSUPPORTED:
		lg("GL_FRAMEBUFFER_UNSUPPORTED");
		break;
	case GL_FRAMEBUFFER_COMPLETE:
		break;
	}

	glBindFramebuffer(GL_FRAMEBUFFER,0);
    
	return true;
}

void OlaGLRenderTarget::release()
{
	if(mHandles[OlaRTParam::COLOR_BUFFER])
	{
		glDeleteTextures(1,(unsigned int*)(mHandles + OlaRTParam::COLOR_BUFFER));
		mHandles[OlaRTParam::COLOR_BUFFER] = 0;
	}

	if(mHandles[OlaRTParam::DEPTH_BUFFER])
	{
		glDeleteRenderbuffers(1,(unsigned int*)(mHandles + OlaRTParam::DEPTH_BUFFER));
		mHandles[OlaRTParam::DEPTH_BUFFER] = 0;
	}

	if(mHandles[OlaRTParam::STENCIL_BUFFER])
	{
		glDeleteRenderbuffers(1,(unsigned int*)(mHandles + OlaRTParam::STENCIL_BUFFER));
		mHandles[OlaRTParam::STENCIL_BUFFER] = 0;
	}

	if(mHandles[OlaRTParam::FRAME_BUFFER])
	{
		glDeleteFramebuffers(1,(unsigned int*)(mHandles + OlaRTParam::FRAME_BUFFER));
		mHandles[OlaRTParam::FRAME_BUFFER] = 0;
	}
}

OlaGLRenderTarget::OlaGLRenderTarget(const char* name ):
OlaRenderTarget(name)
{
	memset(mHandles,0,OlaRTParam::RTPARAM_TYPE_END*sizeof(int));
	mBpp = 8;
}

void* OlaGLRenderTarget::handle()
{
	return &(mHandles[OlaRTParam::COLOR_BUFFER]);
}

OlaGLOriginRT::OlaGLOriginRT():
OlaGLRenderTarget("gl_default_rt"),
mGLDefaultFB(-1)
{

}
OlaGLOriginRT::~OlaGLOriginRT()
{

}

bool OlaGLOriginRT::initialize(int w ,int h)
{
	mWidth = w;
	mHeight = h;

	glGetIntegerv(GL_FRAMEBUFFER_BINDING,&mGLDefaultFB);
	mHandles[OlaRTParam::COLOR_BUFFER] = mGLDefaultFB;
	return true;
}

void OlaGLOriginRT::release()
{

}