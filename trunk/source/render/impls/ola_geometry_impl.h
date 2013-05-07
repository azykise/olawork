#ifndef _OLA_INCLUDE_GEOMETRY_IMPLS_H__
#define _OLA_INCLUDE_GEOMETRY_IMPLS_H__

#include "../ola_util.h"
#include "../../interface/ola_engine.h"

class OlaMaterialImpl;
class OlaMeshRenderer;
class OlaRenderOp;

class OlaMeshGeometryImpl : public ola::IGeometry
{
public:
	OlaMeshGeometryImpl();
	virtual ~OlaMeshGeometryImpl();

	OlaArray<OlaRenderOp*>& renderOps();

protected:
	OlaMeshRenderer* mMesh;
};

#endif