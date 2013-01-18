#pragma once

#include "../../interface/ola_engine.h"

#include <windows.h>

class OlaRenderCore;
class OlaSwapBuffer;
class OlaRenderWindow : public ola::IRenderWindow
{
public:
	OlaRenderWindow(int idx,OlaRenderCore* parent);
	virtual ~OlaRenderWindow();

	virtual void setBackgroundColor(float r,float g,float b,float a);

	virtual void resizeRenderWindow(ola::RenderWindowInfo*);

	virtual void setActive();

	virtual void swapBuffer();

	virtual void release();

	virtual int getWidth() {return mWidth;};
	virtual int getHeight(){return mHeight;};
	virtual int getIndex(){return mIndex;};

protected:

	int mWidth;
	int mHeight;
	int mIndex;

	HWND mWindow;

	float mBackColor[4];

	OlaRenderCore* mParent;
	OlaSwapBuffer* mSwBuffer;
};