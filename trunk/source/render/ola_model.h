#ifndef  _OLAMODEL_MODEL_H__
#define  _OLAMODEL_MODEL_H__

#include "ola_util.h"

#include "ola_kernelobj.h"

class OlaMesh;
class OlaMaterial;
class OlaRenderOp;
class OlaTransformObj;

class OlaMeshRenderer : public OlaKernelObj
{
public:

	OlaMeshRenderer(const char* dmlassetpath);

	virtual ~OlaMeshRenderer();

	virtual OlaMesh* mesh();

	virtual void mesh(OlaMesh* newMesh);

	virtual OlaMaterial* material(int index);

	virtual void material(int index,OlaMaterial* mat);

	virtual const char* kernelID();

	virtual ENABLE_STATE enabled();

	virtual void updateInternal( float elasped , OlaTransformObj* transObj );

	virtual void renderInternal( OlaRender* r );

protected:	

	olastring mDMLAssetpath;

	OlaMesh* mMesh;

	OlaArray<OlaMaterial*> mMaterials;

	OlaArray<OlaRenderOp*> mRenderOps;
};

#endif