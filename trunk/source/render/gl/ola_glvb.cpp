#include <stdio.h>
#include <assert.h>

#if defined WIN32
#include "GLES2/gl2.h"
#elif defined __ANDROID_API__
#include "GLES2/gl2.h"
#elif defined __APPLE__
#include <OpenGLES/ES2/gl.h>
#endif

#include "ola_glvb.h"

void OlaGLVB::release()
{
	mElements.clear();


	if(mBuffer)
	{
		free(mBuffer);
		mBuffer = 0;

		mVertnum = 0;
		mVertstride = 0;
	}

	if(mHandle)
	{
		glDeleteBuffers(1,&mHandle);
		mHandle = 0;
	}

	memset(mElementPos,-1,OlaVBParam::VB_ELEMENT_TYPE_END * sizeof(int));
	memset(mElementOffset,-1,OlaVBParam::VB_ELEMENT_TYPE_END * sizeof(int));
}

void OlaGLVB::unlock()
{
	int l = mVertstride * mVertnum;
	if( l <= 0)
		return;

	if(!mHandle)
		glGenBuffers(1,&mHandle);

	GLenum vbot = GL_STATIC_DRAW;

	switch(mVBOType)
	{
		case OlaVBParam::VBO_STATIC:
			vbot = GL_STATIC_DRAW;
			break;
		case OlaVBParam::VBO_DYNAMIC:
			vbot = GL_DYNAMIC_DRAW;
			break;
	}

	glBindBuffer(GL_ARRAY_BUFFER,mHandle);
	glBufferData(GL_ARRAY_BUFFER, l ,mBuffer,vbot);
	GLenum error = glGetError();
#ifdef GEO_BUFFER_LOG
	if(error != GL_NO_ERROR)
	{
		lg("vb %d created failed \n",mHandle);
		mHandle = 0;
	}
	else
	{
		lg("vb %d has been created \n",mHandle);
	}
#endif
	glBindBuffer(GL_ARRAY_BUFFER,0);
}

void* OlaGLVB::handle()
{
	return &mHandle;
}

OlaGLVB::OlaGLVB():
mHandle(0)
{

}