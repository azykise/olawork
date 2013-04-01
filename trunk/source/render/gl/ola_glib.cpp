#include <stdio.h>
#include <assert.h>

#if defined WIN32
#include "GLES2/gl2.h"
#elif defined __ANDROID_API__
#include "GLES2/gl2.h"
#elif defined __APPLE__
#include <OpenGLES/ES2/gl.h>
#endif

#include "ola_glib.h"

void OlaGLIB::release()
{
	mData.clear();
	if(mHandle)
	{
		glDeleteBuffers(1,&mHandle);
		mHandle = 0;
	}
}

void OlaGLIB::unlock()
{
	GLushort* p = &(mData[0]);
	int num = mData.size();

	if(mHandle == 0)
		glGenBuffers(1,&mHandle);
 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(GLushort) * num ,p,GL_STATIC_DRAW);
	GLenum error = glGetError();
#ifdef GEO_BUFFER_LOG
	if(error != GL_NO_ERROR)
	{
		lg("ib %d created failed \n",mHandle);
		mHandle = 0;
	}
	else
	{
		lg("ib %d has been created \n",mHandle);
		printl3();
	}
#endif
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
}

void* OlaGLIB::handle()
{
	return &mHandle;
}

OlaGLIB::OlaGLIB():
mHandle(0)
{

}