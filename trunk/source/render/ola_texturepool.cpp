#include "ola_texturepool.h"
#include "ola_texture.h"

#include "ola_device.h"

OlaTexturePool::OlaTexturePool():
mDefaultTexture(0)
{
	mDefaultTexture = GetRenderDevice()->spawnTexture();
	mDefaultTexture->fill(DEFAULT_TEXTURE_DATA,2,2,24);
}

OlaTexturePool::~OlaTexturePool()
{
	if (mDefaultTexture)
	{
		delete mDefaultTexture;
		mDefaultTexture = 0;
	}
}

void OlaTexturePool::enPool( const char* texlassetpath, OlaTexture* texture )
{
	olastring _texlassetpath(texlassetpath);
	TexturePool::iterator i = mTexturePool.find(_texlassetpath);
	if (i == mTexturePool.end())
	{
		texture->addRef();
		mTexturePool[_texlassetpath] = texture;
	}
}

void OlaTexturePool::dePool( const char* texlassetpath )
{
	olastring _texlassetpath(texlassetpath);
	TexturePool::iterator i = mTexturePool.find(_texlassetpath);
	if (i != mTexturePool.end())
	{
		OlaTexture* texture = i->second;
		texture->delRef();
		if (texture->refCount() == 0)
		{
			mTexturePool.erase(_texlassetpath);
			delete texture;
		}
	}
}

OlaTexture* OlaTexturePool::seek( const char* texlfilename )
{
	olastring _texlfilename(texlfilename);

	TexturePool::iterator i = mTexturePool.find(_texlfilename);
	if (i != mTexturePool.end())
	{
		return i->second;
	}

	return 0;
}
