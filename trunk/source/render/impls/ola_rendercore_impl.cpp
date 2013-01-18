#include <exception>

#include "../math/ola_math.h"
#include "../ola_device.h"

#include "ola_renderable_impl.h"
#include "ola_rendercore_impl.h"

OlaRenderDevice* gDevice = 0;
OlaRenderDevice* GetRenderDevice()
{
	return gDevice;
}

OlaRenderCoreImpl::OlaRenderCoreImpl():
mRender(0)
{

}

OlaRenderCoreImpl::~OlaRenderCoreImpl()
{

}

const int DEFAULT_W = 320;
const int DEFAULT_H = 480;
bool OlaRenderCoreImpl::initialize( unsigned int hwnd )
{
	gDevice = SpawnGLRenderDevice();
	gDevice->init(hwnd);

	mRender = new OlaRender();
	mRender->onInitRender(DEFAULT_W,DEFAULT_H,gDevice);

	return true;
}

void OlaRenderCoreImpl::release()
{
	if(mRender)
	{
		delete mRender;
		mRender;
	}

	if(gDevice)
	{
		delete gDevice;
		gDevice = 0;
	}
}

void OlaRenderCoreImpl::beginDraw()
{
	throw std::exception("The method or operation is not implemented.");
}

void OlaRenderCoreImpl::drawScene()
{
	throw std::exception("The method or operation is not implemented.");
}

void OlaRenderCoreImpl::endDraw()
{
	throw std::exception("The method or operation is not implemented.");
}

IRenderWindow* OlaRenderCoreImpl::createWindow( RenderWindowInfo* info )
{
	throw std::exception("The method or operation is not implemented.");
}

IPrimitive* OlaRenderCoreImpl::createPrimitive( PrimitiveType type )
{
	throw std::exception("The method or operation is not implemented.");
}

IGeometry* OlaRenderCoreImpl::createGeometry()
{
	throw std::exception("The method or operation is not implemented.");
}

IIllumination* OlaRenderCoreImpl::createIllumination()
{
	throw std::exception("The method or operation is not implemented.");
}

ola::IRenderable* OlaRenderCoreImpl::createRenderable()
{
	OlaRenderableImpl* r = new OlaRenderableImpl(this);
	mRenderables.push_back(r);
	return r;
}

void OlaRenderCoreImpl::releaseObject( IRenderCoreObj* object )
{
	delete object;
}

IMaterial* OlaRenderCoreImpl::createMaterial()
{
	throw std::exception("The method or operation is not implemented.");
}

IShader* OlaRenderCoreImpl::createShader()
{
	throw std::exception("The method or operation is not implemented.");
}

void OlaRenderCoreImpl::cleanUnusedResources( const char* res_name )
{
	throw std::exception("The method or operation is not implemented.");
}

int OlaRenderCoreImpl::resourceNum( const char* res_name )
{
	throw std::exception("The method or operation is not implemented.");
}



