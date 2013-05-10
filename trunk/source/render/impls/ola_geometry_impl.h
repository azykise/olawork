#ifndef _OLA_INCLUDE_GEOMETRY_IMPLS_H__
#define _OLA_INCLUDE_GEOMETRY_IMPLS_H__

#include "../ola_util.h"
#include "../../interface/ola_engine.h"

class OlaMeshRenderer;
class OlaMaterialImpl;
class OlaMesh;
class OlaRenderOp;

class OlaGeometryImpl : public ola::IGeometry
{
public:
	OlaGeometryImpl(OlaMeshRenderer* model);
	virtual ~OlaGeometryImpl();

	virtual int submeshNum(){ return mSubMaterials.size(); };

	virtual ola::IMaterial* submeshMaterial(int idx);

	virtual void reload(){};

	virtual const char* getResourceFilename(){return "";}	

	virtual OlaMeshRenderer* meshrender();

protected:

	OlaMeshRenderer* mModel;
	std::vector<OlaMaterialImpl*> mSubMaterials;
};

#endif