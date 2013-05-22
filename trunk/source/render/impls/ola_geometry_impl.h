#ifndef _OLA_INCLUDE_GEOMETRY_IMPLS_H__
#define _OLA_INCLUDE_GEOMETRY_IMPLS_H__

#include "../ola_util.h"
#include "../../interface/ola_engine.h"

class OlaMeshRenderer;
class OlaMaterialImpl;
class OlaMesh;
class OlaRender;

class OlaGeometryImpl : public ola::IGeometry
{
public:
	OlaGeometryImpl(OlaMeshRenderer* model,OlaRender* render);
	virtual ~OlaGeometryImpl();

	virtual int submeshNum(){ return mSubMaterials.size(); };

	virtual ola::IMaterial* submeshMaterial(int idx);

	virtual void reload();

	virtual bool deserialize(const char* data,int len){return false;}

	virtual const char* filename(){return "";}	

	virtual OlaMeshRenderer* meshrender();

protected:
	OlaRender* mRender;
	OlaMeshRenderer* mModel;
	std::vector<OlaMaterialImpl*> mSubMaterials;
};

#endif