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

	void setWorldTransform(olaMat4& m){mWorldMat = m;}

	void setScale(float x,float y,float z);
	void setRotation(float x,float y,float z,float w);
	void setPostition(float x,float y,float z);

	void setScale(olaVec3& sca);	
	void setRotation(olaQuat& quat);	
	void setPostition(olaVec3& pos);

	olaMat4& getWorldTransform(){return mWorldMat;}

	RenderOpList& renderops() {return mRenderOps;} 

	OlaMesh* mesh(){return mMesh;}

	const char* name(){return mName.c_str();}

protected:

	void _setWorldTransform();

	olastring mName;	

	olaMat4 mWorldMat;

	OlaMesh* mMesh;

	olaVec3 mPosition;
	olaVec3 mScale;
	olaQuat mRotation;

	RenderOpList mRenderOps;
};

#endif