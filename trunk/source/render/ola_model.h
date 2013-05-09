#ifndef  _OLAMODEL_MODEL_H__
#define  _OLAMODEL_MODEL_H__

#include "ola_util.h"

class OlaMesh;
class OlaMaterial;
class OlaRenderOp;
class OlaTransformObj;

class OlaMeshRenderer
{
public:

	OlaMeshRenderer();
	virtual ~OlaMeshRenderer();

	virtual OlaMesh* mesh();
	virtual void mesh(OlaMesh* newMesh);

	virtual OlaMaterial* material(int index);
	virtual void material(int index,OlaMaterial* mat);

	virtual OlaTransformObj* transform();
	virtual void transform(OlaTransformObj* transobj);

	virtual OlaArray<OlaRenderOp*>& updateRenderOps();

protected:	

	OlaMesh* mMesh;

	OlaTransformObj* mTranform;

	OlaArray<OlaMaterial*> mMaterials;

	OlaArray<OlaRenderOp*> mRenderOps;
};

#endif