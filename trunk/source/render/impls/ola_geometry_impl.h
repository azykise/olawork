#ifndef _OLA_INCLUDE_GEOMETRY_IMPLS_H__
#define _OLA_INCLUDE_GEOMETRY_IMPLS_H__

#include "../ola_stl.h"
#include "../../interface/ola_irendercore.h"

class OlaMesh;
class OlaMaterialImpl;
class OlaResourceMng;

class OlaGeometryImpl : public ola::IGeometry
{
public:
	OlaGeometryImpl(OlaResourceMng* res_mng);
	virtual ~OlaGeometryImpl();

	virtual int deserialize(const ola::byte* data,int len);

	virtual int submeshNum();

	virtual ola::IMaterial* material(int idx);

	virtual void setMaterial( int idx , ola::IMaterial* mat );

	virtual ola::tAABB* aabb();

	virtual void setRootBone( ola::ITransform* rb);

	virtual ola::ITransform* rootbone();

	inline OlaMesh* mesh(){return mMesh;}

	OlaResourceMng* mResourceMng;
	OlaMesh* mMesh;
	OlaArray<OlaMaterialImpl*> mSubMaterials;
};

#endif