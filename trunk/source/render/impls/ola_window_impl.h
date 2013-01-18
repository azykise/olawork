#ifndef _OLA_INCLUDE_RENDERCORE_IMPLS_H__
#define _OLA_INCLUDE_RENDERCORE_IMPLS_H__

#include "../../interface/ola_irendercore.h"

class OlaRenderCore;
class OlaSwapBuffer;
class OlaWindowImpl : public ola::IRenderWindow
{
public:
	OlaWindowImpl(OlaRenderCore* parent);

	virtual ~OlaWindowImpl();

	virtual void setBackgroundColor(float r,float g,float b,float a);

	virtual void resize( int w,int h );

	virtual void setActive( bool a );

	virtual void swapBuffer();

	virtual int width();

	virtual int height();

	virtual bool isActived();

	virtual ola::ICamera* cameras( int* num );

protected:

	int mWidth;
	int mHeight;
	int mIndex;

	unsigned mWindow;

	float mBackColor[4];

	bool mActive;
	OlaRenderCore* mParent;
	OlaSwapBuffer* mSwBuffer;
};

#endif
