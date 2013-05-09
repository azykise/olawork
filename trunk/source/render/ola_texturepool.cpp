#include "ola_texturepool.h"
#include "ola_texture.h"

OlaTexturePool::OlaTexturePool()
{

}

OlaTexturePool::~OlaTexturePool()
{

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
		i->second->addRef();
		return i->second;
	}

	return 0;
}
