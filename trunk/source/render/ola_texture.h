#ifndef _OLA_TEXTURE_H__
#define _OLA_TEXTURE_H__

#include "ola_resource.h"

class OlaAsset;
class OlaTexture : public IRefCounter
{
public:
	enum TEXTURE_TYPE
	{
		COLOR_UNKNOWN = 0,	// unknown
		COLOR_INDEX,		// palette
		COLOR_GRAY,
		COLOR_RGB,
		COLOR_RGBA,
	};
public:
	OlaTexture();
	virtual ~OlaTexture();

	virtual void release() = 0;

	virtual int	 readTga(char* data,int len);
	virtual int  readTga(OlaAsset* asset);
	virtual bool fill(const unsigned char* rgba,int w,int h,int bpp) = 0;
	virtual bool filltest();

	virtual void* handle() = 0;

	int width()					{ return mWidth;}
	int height()				{ return mHeight;}
	int bpp()					{ return mBpp;} //bit depth
	int size()					{ return mWidth * mHeight * mBpp/8;} // byte	
	unsigned char* rgba()		{ return mRGBA;}

protected:
	int mWidth;
	int mHeight;
	int mBpp; //bit per pixel
	unsigned char* mRGBA;
	
	TEXTURE_TYPE mType;
};

#endif