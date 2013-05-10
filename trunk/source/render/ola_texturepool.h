#ifndef _OLA_TEXTURE_POOL_H__
#define _OLA_TEXTURE_POOL_H__

#include "ola_util.h"

class OlaTexture;
class OlaTexturePool
{
public:
	typedef OlaMap<olastring,OlaTexture*> TexturePool;
public:
	OlaTexturePool();

	virtual ~OlaTexturePool();

	void enPool(const char* texlassetpath,OlaTexture* texture);
	void dePool(const char* texlassetpath);

	OlaTexture* seek(const char* texlassetpath);	

	OlaTexture* deftexture(){return mDefaultTexture;};

protected:
	TexturePool mTexturePool;

	OlaTexture* mDefaultTexture;
};

#endif