#ifndef _OLA_INCLUDE_RENDERABLE_IMPLS_H__
#define _OLA_INCLUDE_RENDERABLE_IMPLS_H__

#include "../ola_stl.h"
#include "../math/ola_math.h"

#include "../../interface/ola_irendercore.h"

class OlaRenderOp;
class OlaGeometryImpl;
class OlaRenderCoreImpl;

class OlaRenderableImpl : public ola::IRenderable
{
public:
	typedef OlaArray<OlaRenderOp*> RenderOpList;
public:
	OlaRenderableImpl(OlaRenderCoreImpl* parent);
	virtual ~OlaRenderableImpl();	

	virtual void setTransform( float* mat4_4 );

	virtual void setGeometry( ola::IGeometry* geo );

	virtual ola::IGeometry* geometry();

	virtual void setActive( bool a );

	virtual bool isActive();

protected:
	void _clearRenderOps();

public:

	bool mActive;
	RenderOpList mRenderOps;
	OlaGeometryImpl* mGeometry;
	OlaRenderCoreImpl* mParent;

	olaMat4 mTransform;
};

#endif