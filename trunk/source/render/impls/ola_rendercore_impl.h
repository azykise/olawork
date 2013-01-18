#ifndef _OLA_INCLUDE_RENDERCORE_IMPLS_H__
#define _OLA_INCLUDE_RENDERCORE_IMPLS_H__

#include "../../interface/ola_irendercore.h"
using namespace ola;

#include "../ola_stl.h"

#include "../ola_render.h"

class OlaRenderableImpl;
class OlaRender;

class OlaRenderCoreImpl : public ola::IRenderCore
{
public:
	typedef OlaArray<OlaRenderableImpl*> RenderableList;
public:

	OlaRenderCoreImpl();

	virtual ~OlaRenderCoreImpl();

	virtual bool initialize( unsigned int hwnd );

	virtual void release();

	virtual void beginDraw();

	virtual void drawScene();

	virtual void endDraw();

	virtual IRenderWindow* createWindow( RenderWindowInfo* info );

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
	RenderableList mRenderables;
	OlaRender* mRender;
};

#endif