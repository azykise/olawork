#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "win_renderwindow.h"
#include "win_rendercore.h"

#include "../ola_device.h"

OlaRenderWindow::OlaRenderWindow(int idx,OlaRenderCore* parent):
mIndex(idx),
mWidth(0),
mHeight(0),
mWindow(0),
mParent(parent),
mSwBuffer(0)
{
	mSwBuffer = parent->device()->spawnSwapBuffer();
}

OlaRenderWindow::~OlaRenderWindow()
{
	
}

void OlaRenderWindow::release()
{
	if(mSwBuffer)
	{
		delete mSwBuffer;
	}
}

void OlaRenderWindow::setBackgroundColor(float r,float g,float b,float a)
{
	mBackColor[0] = r;	mBackColor[1] = g;
	mBackColor[2] = b;	mBackColor[3] = a;
}

void OlaRenderWindow::resizeRenderWindow(ola::RenderWindowInfo* winfo)
{
	HWND hw = (HWND)(winfo->hwnd);
	if(mWindow != hw || mWidth != winfo->width || mHeight != winfo->height)
	{
		mWindow = hw;
		mWidth = winfo->width;
		mHeight = winfo->height;
	}
}

void OlaRenderWindow::setActive()
{
	if (!mWindow)
	{
		return;
	}

	if(mSwBuffer)
	{
		mSwBuffer->setScreenHandle(&mWindow);
	}

	mParent->device()->setSwapBuffer(mSwBuffer);
}

void OlaRenderWindow::swapBuffer()
{
	if(mSwBuffer)
	{
		mSwBuffer->swapBuffer();
	}
}


