#ifndef  _TEST_MODEL_H__
#define  _TEST_MODEL_H__

#include "ola_util.h"
#include "ola_render.h"

#include "math/ola_math.h"
#include "ola_mesh.h"
#include "ola_material.h"
#include "ola_string.h"

class CModel
{
public:
	typedef std::vector<OlaRenderOp*> RenderOpList;
public:
	CModel(olastring& name,OlaMesh* mesh);

	virtual ~CModel();

	RenderOpList& renderops() {return mRenderOps;} 

	OlaMesh* mesh(){return mMesh;}	

	OlaMesh* mMesh;

	RenderOpList mRenderOps;
};

#endif