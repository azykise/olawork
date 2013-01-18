#include <exception>

#include "../ola_device.h"
#include "ola_window_impl.h"

OlaWindowImpl::OlaWindowImpl( OlaRenderCore* parent ):
mActive(false)
{

}

OlaWindowImpl::~OlaWindowImpl()
{

}

int OlaWindowImpl::width()
{
	throw std::exception("The method or operation is not implemented.");
}

int OlaWindowImpl::height()
{
	throw std::exception("The method or operation is not implemented.");
}

void OlaWindowImpl::setBackgroundColor( float r,float g,float b,float a )
{

}

void OlaWindowImpl::resize( int w,int h )
{
	throw std::exception("The method or operation is not implemented.");
}

void OlaWindowImpl::setActive( bool a )
{
	mActive = a;
	if (mActive)
	{
		GetRenderDevice()->setSwapBuffer(this->mSwBuffer);
	}	
}

void OlaWindowImpl::swapBuffer()
{
	if (mActive)
	{
		mSwBuffer->swapBuffer();
	}
}

bool OlaWindowImpl::isActived()
{
	return mActive;
}

ola::ICamera* OlaWindowImpl::cameras( int* num )
{
	*num = 0;
	return 0;
}

