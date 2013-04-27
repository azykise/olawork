#include "ola_render.h"
#include "model.h"

CModel::CModel(olastring& name,OlaMesh* mesh):
mName(name),
mMesh(0)
{
	mMesh = mesh;
	mWorldMat.Identity();
	olaVec3 v(0,0,0);
	mWorldMat.TransformCoord(v);

	OlaMesh::SubMeshList& submeh_list = mMesh->submeshs();
	OlaMesh::SubMeshList::iterator i = submeh_list.begin();
	while(i != submeh_list.end())
	{
		OlaRenderOp* op = new OlaRenderOp(*i);
		mRenderOps.push_back(op);
		i++;
	}
	mMesh->addRef();
}

CModel::~CModel()
{
	for (int i = 0 ; i < mRenderOps.size() ; i++)
	{
		OlaRenderOp* op = mRenderOps.at(i);
		delete op;
	}
	mRenderOps.clear();

	if(mMesh)
	{
		mMesh->delRef();
		mMesh = 0;
	}
}

static olaMat4 _sca_mat;
static olaMat4 _rot_mat;
static olaMat4 _pos_mat;

void CModel::_setWorldTransform()
{
	_sca_mat.Identity();
	_sca_mat[0][0] = mScale.x;
	_sca_mat[1][1] = mScale.y;
	_sca_mat[2][2] = mScale.z;
	_sca_mat[3][3] = 1.0f;

	_rot_mat = mRotation.ToMat4();

	_pos_mat.Identity();
	_pos_mat[0][3] = mPosition.x;
	_pos_mat[1][3] = mPosition.y;
	_pos_mat[2][3] = mPosition.z;
	_pos_mat[3][3] = 1.0f;

	mWorldMat = _pos_mat * _sca_mat * _rot_mat;
}

void CModel::setScale(olaVec3& sca)
{
	mScale = sca;
	_setWorldTransform();
}

void CModel::setRotation(olaQuat& quat)
{
	mRotation = quat;
	_setWorldTransform();
}

void CModel::setPostition(olaVec3& pos)
{
	mPosition = pos;
	_setWorldTransform();
}

static olaVec3 temp_vec3;
static olaQuat temp_quat;
void CModel::setScale(float x,float y,float z)
{
	temp_vec3.Set(x,y,z);
	setScale(temp_vec3);
}
void CModel::setRotation(float x,float y,float z,float w)
{
	temp_quat.Set(x,y,z,w);
	temp_quat.Normalize();
	setRotation(temp_quat);
}
void CModel::setPostition(float x,float y,float z)
{
	temp_vec3.Set(x,y,z);	
	setPostition(temp_vec3);
}

