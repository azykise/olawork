#include "ola_render.h"
#include "model.h"

CModel::CModel(olastring& name,OlaMesh* mesh):
mMesh(0)
{
	mMesh = mesh;
	mMesh->addRef();

	//OlaMesh::SubMeshList& submeh_list = mMesh->submeshs();
	//OlaMesh::SubMeshList::iterator i = submeh_list.begin();
	//while(i != submeh_list.end())
	//{
	//	OlaRenderOp* op = new OlaRenderOp(*i);
	//	mRenderOps.push_back(op);
	//	i++;
	//}
	
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
