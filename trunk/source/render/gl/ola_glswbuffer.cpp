#include <assert.h>

#if defined WIN32
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#elif defined __ANDROID_API__
#include <GLES2/gl2.h>
#elif defined __APPLE__
#include <OpenGLES/ES2/gl.h>
#endif

#include "ola_glswbuffer.h"

OlaEGLBufferWin::OlaEGLBufferWin(void* ph,OlaGLDevice* device):
mDevice(device),
mEGLSurface(EGL_NO_SURFACE),
mEGLContext(EGL_NO_CONTEXT),
mHWND(0)
{
	EGLint aEGLContextAttributes[] = {
		EGL_CONTEXT_CLIENT_VERSION, 2,
		EGL_NONE
	};

	mEGLContext = eglCreateContext(mDevice->glDisplay(), mDevice->glConfigs()[0], device->glContext(), aEGLContextAttributes);
	assert (mEGLContext != EGL_NO_CONTEXT && "Failed to create EGL context.\n");

	mHWND = *static_cast<unsigned int*>(ph);

	mEGLSurface = eglCreateWindowSurface(mDevice->glDisplay(), mDevice->glConfigs()[0], (EGLNativeWindowType)mHWND , 0);
	assert(mEGLSurface != EGL_NO_SURFACE && "Failed to create EGL surface.\n");	
}

OlaEGLBufferWin::~OlaEGLBufferWin()
{
	if (mEGLSurface)
	{
		eglDestroySurface(mDevice->glDisplay(),mEGLSurface);
		mEGLSurface = 0;
		mHWND = 0;
	}

	if (mEGLContext)
	{
		eglDestroyContext(mDevice->glDisplay(),mEGLContext);
		mEGLContext = 0;
	}
}

//void OlaEGLBufferWin::setScreenHandle( void* ph )
//{
//	unsigned int* p = (unsigned int*)ph;
//	if (mHWND == *p)
//	{
//		return;
//	}
//
//	mHWND = *p;
//
//	if (mEGLSurface)
//	{
//		eglDestroySurface(mDevice->glDisplay(),mEGLSurface);
//		mEGLSurface = 0;
//		mHWND = 0;
//	}
//
//	mEGLSurface = eglCreateWindowSurface(mDevice->glDisplay(), mDevice->glConfigs()[0], (EGLNativeWindowType)mHWND , 0);
//	assert(mEGLSurface != EGL_NO_SURFACE && "Failed to create EGL surface.\n");	
//}

void OlaEGLBufferWin::resize( int x,int y )
{
	
}

void OlaEGLBufferWin::swapBuffer()
{
    if (!eglSwapBuffers(mDevice->glDisplay(), mEGLSurface)) {
        //lg("Failed to swap buffers.\n");
    }	
}

void* OlaEGLBufferWin::handle()
{
	return mEGLSurface;
}



