#ifndef _OLA_INCLUDE_RENDERCORE_IMPLS_H__
#define _OLA_INCLUDE_RENDERCORE_IMPLS_H__

#include "../../interface/ola_irendercore.h"
using namespace ola;

#include "../ola_stl.h"

#include "../ola_render.h"

class OlaRenderableImpl;
class OlaRender;

class OlaHardwareEnvImpl : public ola::IHardwareEnvironment
{
public:
	virtual ~OlaHardwareEnvImpl();

	virtual void setHardwareHandle( unsigned int hwnd );

	virtual ola::HARDWARE_TYPE hardwareType();
};

class OlaRenderCoreImpl : public ola::IRenderCore
{
public:
	typedef OlaArray<OlaRenderableImpl*> RenderableList;
public:

	OlaRenderCoreImpl();

	virtual IHardwareEnvironment* createHardware( HARDWARE_TYPE hwt );

	virtual ~OlaRenderCoreImpl();

	virtual bool initialize();

	virtual void release();

	virtual void beginDraw();

	virtual void drawScene();

	virtual void endDraw();

	virtual IDrawSurface* createDrawSurface();

	virtual IPrimitive* createPrimitive( PrimitiveType type );

	virtual IGeometry* createGeometry();

	virtual IIllumination* createIllumination();

	virtual IRenderable* createRenderable();

	virtual void releaseObject( IRenderCoreObj* object );

	virtual IMaterial* createMaterial();

	virtual IShader* createShader();

	virtual void cleanUnusedResources( const char* res_name );

	virtual int resourceNum( const char* res_name );

	inline RenderableList& renderables(){return mRenderables;}	

protected:
	OlaHardwareEnvImpl* mDeviceEnv;
	RenderableList mRenderables;
	OlaRender* mRender;
};

#endif