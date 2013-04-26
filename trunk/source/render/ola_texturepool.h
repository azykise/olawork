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

	void enPool(OlaTexture* texture);
	void dePool(OlaTexture* texture);
	void dePool(const char* texlfilename);

	OlaTexture* seek(const char* texlfilename);	

protected:
	TexturePool mMaterialPool;
};

#endif