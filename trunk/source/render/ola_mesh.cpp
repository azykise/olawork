#include "ola_render.h"

#include "ola_mesh.h"
#include "math/ola_math.h"
#include "ola_ase.h"
#include "ola_resourcemng.h"

#include "ola_skeleton.h"
#include "ola_device.h"

struct tOlaVertex
{
	float pos[3];
	float nor[3];
	float uv0[2];
	float tan[3];
	float bnr[3];
};

OlaMesh::OlaMesh(const char* name):
mName(name)
{
	
}

OlaMesh::~OlaMesh()
{
	clear();
}

void OlaMesh::clear()
{
	SubMeshList::iterator sub_i = mSubMeshs.begin();
	while (sub_i != mSubMeshs.end())
	{
		(*sub_i)->clear();
		delete *sub_i;
		sub_i++;
	}
	mSubMeshs.clear();
}

bool OlaMesh::copyTo(OlaMesh* mesh,OlaVBParam::VBO_TYPE vb_type)
{
	mesh->clear();

	for (int i = 0 ; i < mSubMeshs.size() ; i++)
	{
		OlaSubMesh* s = mSubMeshs[i];

		OlaSubMesh* c = new OlaSubMesh(s->name().c_str());

		s->copyTo(c,vb_type);

		mesh->submeshs().push_back(c);
	}

	return true;
}

OlaASE::matobj* GetMaterialLeaf(OlaASE::matobj* matnode, int submat_id)
{
	if (!matnode) return 0;

	if (matnode->classname)
	{
		if (!strcmp(matnode->classname,"\"Multi/Sub-Object\""))
			return GetMaterialLeaf(matnode->submat[submat_id],submat_id);
	}

	if (!matnode->submat.size() || submat_id >= matnode->submat.size() )//no sub material , it's a standard material
		return matnode;

	return GetMaterialLeaf(matnode->submat[submat_id],submat_id);
}

const char*	GetMeshShaderName(int matid, int submat_id, OlaASE::mat_array& matlib)
{
	if (matid<0)//no material on it
		return 0;

	OlaASE::matobj* mat = 0;
	OlaASE::matobj* matnode = matlib[matid];


	//get the root material node
	mat = GetMaterialLeaf(matnode,submat_id);

	if (!mat)
		return 0;

	return mat->name;
}

OlaSubMesh::OlaSubMesh(const char* name):
mName(name),
mMaterial(0),
mVB(0),
mIB(0)
{
	mVB = GetRenderDevice()->spawnVB();
	mIB = GetRenderDevice()->spawnIB();

	mMinVertex[0] = mMinVertex[1] = mMinVertex[2] = olaMath::INFINITY;
	mMaxVertex[0] = mMaxVertex[1] = mMaxVertex[2] = -1.0f * olaMath::INFINITY;
}

OlaSubMesh::~OlaSubMesh()
{
	if(mVB)
	{
		delete mVB;
		mVB = 0;
	}
	
	if(mIB)
	{
		delete mIB;
		mIB = 0;
	}	
}

void OlaSubMesh::clear()
{
	mVB->release();
	mIB->release();

	if (mMaterial)
	{
		mMaterial->delRef();
		mMaterial = 0;
	}

	for (size_t i = 0 ; i < mSkeletonBones.size() ; i++)
	{
		delete mSkeletonBones[i];
	}
	mSkeletonBones.clear();
}

void OlaSubMesh::copyTo(OlaSubMesh* submesh,OlaVBParam::VBO_TYPE vb_type)
{
	submesh->clear();

	submesh->setMaterial(mMaterial);
	if(mVertexSkinInfos.size() != 0)
	{
		submesh->mVertexSkinInfos.resize(mVertexSkinInfos.size());
		memcpy(&(submesh->mVertexSkinInfos[0]),&(mVertexSkinInfos[0]),sizeof(tVSkinInfo) * mVertexSkinInfos.size());
	}
	
	if(mSkeletonBones.size() != 0)
	{
		submesh->mSkeletonBones.resize(mSkeletonBones.size());
		for (size_t i = 0 ; i < mSkeletonBones.size() ; i++)
		{
			tSBone* sbone = new tSBone();
			sbone->id_ase = mSkeletonBones[i]->id_ase;
			sbone->name = mSkeletonBones[i]->name;
			submesh->mSkeletonBones[i] = sbone;	
		}
	}

	memcpy(submesh->mMinVertex,mMinVertex,sizeof(float) * 3);
	memcpy(submesh->mMaxVertex,mMaxVertex,sizeof(float) * 3);

	int num_vert = mVB->num_v();

	submesh->mVB->initialize(mVB->elements(),num_vert,vb_type);
	tOlaVertex* vert_data_src = (tOlaVertex*)(mVB->lock());
	tOlaVertex* vert_data_dst = (tOlaVertex*)(submesh->mVB->lock());
	memcpy(vert_data_dst,vert_data_src,sizeof(tOlaVertex) * num_vert);
	submesh->mVB->unlock();

	submesh->mIB->initialize(num_vert);
	unsigned short* index_src = mIB->lock();
	unsigned short* index_dst = submesh->mIB->lock();
	memcpy(index_dst,index_src,sizeof(unsigned short) * num_vert);
	submesh->mIB->unlock();
}

void OlaSubMesh::setMaterial( OlaMaterial* mat )
{
	if (mMaterial)	
		mMaterial->delRef();	

	mMaterial = mat;
	mMaterial->addRef();
}
