#include <stdio.h>
#include <stdlib.h>

#include "egl_surface.h"

static void* mEGLDisplay;
static void* mEGLSurface;
static void* mEGLContext;

#define  lg(...) printf(__VA_ARGS__)

void createEglSurface(EGLNativeWindowType hwindow)
{
	EGLint majorVersion;
	EGLint minorVersion;
	mEGLDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	if(mEGLDisplay == EGL_NO_DISPLAY)
		lg("Unable to open connection to local windowing system \n");

	if(!eglInitialize(mEGLDisplay, &majorVersion, &minorVersion))
		lg("Unable to initialize EGL. Handle and recover \n");
	else
		lg("EGL initilzed : verison %d  \n",majorVersion);

    EGLint aEGLAttributes[] = {
        EGL_RED_SIZE, 5,
        EGL_GREEN_SIZE, 6,
        EGL_BLUE_SIZE, 7,
        EGL_DEPTH_SIZE, 16,
		EGL_SURFACE_TYPE,
		EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, 
		EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };

    EGLint aEGLContextAttributes[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

	EGLConfig	aEGLConfigs[1];
	EGLint		cEGLConfigs;

	if(!eglChooseConfig(mEGLDisplay, aEGLAttributes, aEGLConfigs, 1, &cEGLConfigs))
		lg("");
	else
		lg("");

    if (cEGLConfigs == 0) {
        lg("No EGL configurations were returned.\n");
        exit(-1);
    }

	mEGLSurface = eglCreateWindowSurface(mEGLDisplay, aEGLConfigs[0], (EGLNativeWindowType)hwindow, 0);
    if (mEGLSurface == EGL_NO_SURFACE) {
        lg("Failed to create EGL surface.\n");
        exit(-1);
    }

	mEGLContext = eglCreateContext(mEGLDisplay, aEGLConfigs[0], EGL_NO_CONTEXT, aEGLContextAttributes);
    if (mEGLContext == EGL_NO_CONTEXT) {
        lg("Failed to create EGL context.\n");
        exit(-1);
    }

	if(!eglMakeCurrent(mEGLDisplay, mEGLSurface, mEGLSurface, mEGLContext))
		lg("eglMakeCurrent Failed \n !!!");	

	lg("EglWindow Inited\n");
}

void swapEglBuffer()
{
    if (mEGLDisplay && mEGLSurface && !eglSwapBuffers(mEGLDisplay, mEGLSurface)) {
        lg("Failed to swap buffers.\n");
    }
}