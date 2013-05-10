#ifndef _OLA_GL_TEXTURE_BUFFER_H__
#define _OLA_GL_TEXTURE_BUFFER_H__

#include "../ola_texture.h"

class OlaGLTexture : public OlaTexture
{
public:
	OlaGLTexture();

	virtual void release();

	virtual bool fill( const unsigned char* rgba,int w,int h,int bpp );

	virtual void* handle();

	unsigned int mHandle;

};

#endif