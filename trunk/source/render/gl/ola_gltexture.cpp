#include <stdio.h>
#include <assert.h>

#if defined WIN32
#include "GLES2/gl2.h"
#elif defined __ANDROID_API__
#include "GLES2/gl2.h"
#elif defined __APPLE__
#include <OpenGLES/ES2/gl.h>
#endif

#include "../ola_util.h"
#include "ola_gltexture.h"

OlaGLTexture::OlaGLTexture():
mHandle(0)
{

}

void OlaGLTexture::release()
{
	if (mRGBA)
	{
		delete [] mRGBA;
		mRGBA = 0;
	}	

	if(mHandle)
	{
		glDeleteTextures(1,&mHandle);
	}
}

bool OlaGLTexture::fill( unsigned char* rgba,int w,int h,int bpp )
{
	release();

	mWidth = w;
	mHeight = h;
	mBpp = bpp;

	int comp = mBpp / 8;

	GLint type = comp == 4 ? GL_RGBA : GL_RGB;
	mType = comp == 4 ? COLOR_RGBA : COLOR_RGB;

	mRGBA = new unsigned char[mWidth * mHeight * comp];
	memcpy(mRGBA,rgba,mWidth * mHeight * comp);	

    // Use tightly packed datahao1~
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	if(glGetError() != GL_NO_ERROR)
	{
		lg("glPixelStorei(GL_UNPACK_ALIGNMENT, 1); \n");
	}
    // Generate a texture object
    glGenTextures(1, &mHandle);
	if(glGetError() != GL_NO_ERROR)
	{
		lg("glGenTextures(1, &mHandle); \n");
	}
    // Bind the texture object
    glBindTexture(GL_TEXTURE_2D, mHandle);
	if(glGetError() != GL_NO_ERROR)
	{
		lg("glBindTexture(GL_TEXTURE_2D, mHandle); \n");
	}
    // Load the texture
    glTexImage2D(GL_TEXTURE_2D, 0, type, mWidth, mHeight, 0, type, GL_UNSIGNED_BYTE, mRGBA);
	if(glGetError() != GL_NO_ERROR)
	{
		lg("glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, mRGBA);\n");
	}
	glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

void* OlaGLTexture::handle()
{
	return &mHandle;
}

