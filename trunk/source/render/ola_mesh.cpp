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

typedef std::map<int,const char*> SubMaterialTable;
bool OlaMesh::load(OlaAsset* asset)
{
	clear();

	OlaASE::model* model = OlaASE::load((char*)asset->data,asset->length,1.0f);

	if(!model || model->geom.size() == 0)
	{
		lg("ase parse failed , asset == 0\n");
		return false;
	}

	SubMaterialTable submats; //find out the sub materials , every submat will subdevision a submesh

	OlaASE::geomobj* geo0 = model->geom[0]; //art spec: one ase file have only one mesh inside

	for (int i = 0 ; i < geo0->numf ; i++)
	{
		int submat_id = geo0->fsubmat[i];

		SubMaterialTable::iterator submat_i = submats.find(submat_id);
		if (submat_i == submats.end())
		{
			const char* shadername = GetMeshShaderName(geo0->matidx,submat_id,model->mat);
			submats[submat_id] = shadername;
			//submats
		}
		else
			continue;
	}

	SubMaterialTable::iterator submat_i = submats.begin();
	while(submat_i != submats.end())
	{
		int submat_id = submat_i->first;
		const char* submat_name = submat_i->second;		

		OlaSubMesh::MeshFaceList faces;

		for (int i = 0; i < geo0->numf ; ++i)
		{
			int face_submat_id = geo0->fsubmat[i];
			if(face_submat_id == submat_id)
			{
				faces.push_back(i);
			}
		}

		olastring _submesh_name(submat_name);
		olastring submesh_name(_submesh_name.c_str(),1,_submesh_name.length() - 1);

		OlaSubMesh* submesh = new OlaSubMesh(submesh_name.c_str());
		submesh->load(geo0,faces);
		mSubMeshs.push_back(submesh);

		submat_i++;
	}
	
	delete model;

	return true;
}

OlaSubMesh::OlaSubMesh(const char* name):
mName(name),
mOrgMaterial(0),
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

	if (mOrgMaterial)
	{
		mOrgMaterial->delRef();
		mOrgMaterial = 0;
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

	submesh->setOrgMaterial(mOrgMaterial);
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

void copyVBoneToSkinInfo(OlaASE::vbonedata* vb,OlaSubMesh::tVSkinInfo* sk)
{
	sk->bone_id[0] = vb->bone_id[0];
	sk->bone_id[1] = vb->bone_id[1];
	sk->bone_id[2] = vb->bone_id[2];
	sk->bone_id[3] = vb->bone_id[3];

	sk->weight[0] = vb->weight[0];
	sk->weight[1] = vb->weight[1];
	sk->weight[2] = vb->weight[2];
	sk->weight[3] = vb->weight[3];
}

bool OlaSubMesh::load(OlaASE::geomobj* geo0,MeshFaceList& faces)
{
	int num_face = faces.size();
	int num_vert = num_face * 3;

	assert( geo0->v && geo0->n && geo0->t && 
			geo0->bn && geo0->tn && "error mesh import!");

	OlaVB::ElementList elements;
	elements.push_back(OlaVBParam::ELEMENT_POS);
	elements.push_back(OlaVBParam::ELEMENT_NOR);
	elements.push_back(OlaVBParam::ELEMENT_UV0);
	elements.push_back(OlaVBParam::ELEMENT_TAN);
	elements.push_back(OlaVBParam::ELEMENT_BNR);

	mVB->initialize(elements,num_vert,OlaVBParam::VBO_STATIC);
	tOlaVertex* vert_data = (tOlaVertex*)(mVB->lock());

	for (int i = 0 ; i < num_face ; i++)
	{
		int face_index = faces[i] * 3;
		int idx = i * 3;
		int idx_v0 = geo0->f[face_index + 0] * 3;
		int idx_v1 = geo0->f[face_index + 1] * 3;
		int idx_v2 = geo0->f[face_index + 2] * 3;

		vert_data[idx + 0].pos[0] = geo0->v[idx_v0 + 0];
		vert_data[idx + 0].pos[1] = geo0->v[idx_v0 + 1];
		vert_data[idx + 0].pos[2] = geo0->v[idx_v0 + 2];

		vert_data[idx + 1].pos[0] = geo0->v[idx_v1 + 0];
		vert_data[idx + 1].pos[1] = geo0->v[idx_v1 + 1];
		vert_data[idx + 1].pos[2] = geo0->v[idx_v1 + 2];

		vert_data[idx + 2].pos[0] = geo0->v[idx_v2 + 0];
		vert_data[idx + 2].pos[1] = geo0->v[idx_v2 + 1];
		vert_data[idx + 2].pos[2] = geo0->v[idx_v2 + 2];
	}

	for (int i = 0 ; i < num_face ; i++)
	{
		int face_index = faces[i] * 3;
		int idx = i * 3;
		int idx_uv0 = geo0->tf[face_index + 0] * 3;
		int idx_uv1 = geo0->tf[face_index + 1] * 3;
		int idx_uv2 = geo0->tf[face_index + 2] * 3;

		vert_data[idx + 0].uv0[0] = geo0->t[idx_uv0 + 0];
		vert_data[idx + 0].uv0[1] = geo0->t[idx_uv0 + 1];

		vert_data[idx + 1].uv0[0] = geo0->t[idx_uv1 + 0];
		vert_data[idx + 1].uv0[1] = geo0->t[idx_uv1 + 1];

		vert_data[idx + 2].uv0[0] = geo0->t[idx_uv2 + 0];
		vert_data[idx + 2].uv0[1] = geo0->t[idx_uv2 + 1];
	}

	for (int i = 0 ; i < num_face ; i++)
	{
		int face_index = faces[i] * 3;
		int idx = i * 3;
		int idx_n0 = geo0->fvn[face_index + 0] * 3;
		int idx_n1 = geo0->fvn[face_index + 1] * 3;
		int idx_n2 = geo0->fvn[face_index + 2] * 3;

		//--
		vert_data[idx + 0].nor[0] = geo0->n[idx_n0 + 0];
		vert_data[idx + 0].nor[1] = geo0->n[idx_n0 + 1];
		vert_data[idx + 0].nor[2] = geo0->n[idx_n0 + 2];

		vert_data[idx + 0].tan[0] = geo0->tn[idx_n0 + 0];
		vert_data[idx + 0].tan[1] = geo0->tn[idx_n0 + 1];
		vert_data[idx + 0].tan[2] = geo0->tn[idx_n0 + 2];

		vert_data[idx + 0].bnr[0] = geo0->bn[idx_n0 + 0];
		vert_data[idx + 0].bnr[1] = geo0->bn[idx_n0 + 1];
		vert_data[idx + 0].bnr[2] = geo0->bn[idx_n0 + 2];

		//--
		vert_data[idx + 1].nor[0] = geo0->n[idx_n1 + 0];
		vert_data[idx + 1].nor[1] = geo0->n[idx_n1 + 1];
		vert_data[idx + 1].nor[2] = geo0->n[idx_n1 + 2];

		vert_data[idx + 1].tan[0] = geo0->tn[idx_n1 + 0];
		vert_data[idx + 1].tan[1] = geo0->tn[idx_n1 + 1];
		vert_data[idx + 1].tan[2] = geo0->tn[idx_n1 + 2];

		vert_data[idx + 1].bnr[0] = geo0->bn[idx_n1 + 0];
		vert_data[idx + 1].bnr[1] = geo0->bn[idx_n1 + 1];
		vert_data[idx + 1].bnr[2] = geo0->bn[idx_n1 + 2];

		//--
		vert_data[idx + 2].nor[0] = geo0->n[idx_n2 + 0];
		vert_data[idx + 2].nor[1] = geo0->n[idx_n2 + 1];
		vert_data[idx + 2].nor[2] = geo0->n[idx_n2 + 2];

		vert_data[idx + 2].tan[0] = geo0->tn[idx_n2 + 0];
		vert_data[idx + 2].tan[1] = geo0->tn[idx_n2 + 1];
		vert_data[idx + 2].tan[2] = geo0->tn[idx_n2 + 2];

		vert_data[idx + 2].bnr[0] = geo0->bn[idx_n2 + 0];
		vert_data[idx + 2].bnr[1] = geo0->bn[idx_n2 + 1];
		vert_data[idx + 2].bnr[2] = geo0->bn[idx_n2 + 2];
	}	

	mVB->unlock();

	mIB->initialize(num_vert);
	unsigned short* index = mIB->lock();
	for (int i = 0 ; i < num_vert ; i++)
	{
		index[i] = i;
	}
	mIB->unlock();

	if(geo0->numvbone != 0 && geo0->numskelbone != 0)
	{
		mVertexSkinInfos.resize(num_vert);
		for (int i = 0 ; i < num_face ; i++)
		{
			int face_index = faces[i] * 3;
			int idx = i * 3;
			int idx_v0 = geo0->f[face_index + 0];
			int idx_v1 = geo0->f[face_index + 1];
			int idx_v2 = geo0->f[face_index + 2];

			tVSkinInfo s0,s1,s2;

			OlaASE::vbonedata& vbone0 = geo0->vbones[idx_v0];
			copyVBoneToSkinInfo(&vbone0,&s0);

			OlaASE::vbonedata& vbone1 = geo0->vbones[idx_v1];
			copyVBoneToSkinInfo(&vbone1,&s1);

			OlaASE::vbonedata& vbone2 = geo0->vbones[idx_v2];
			copyVBoneToSkinInfo(&vbone2,&s2);

			mVertexSkinInfos[idx + 0] = s0;
			mVertexSkinInfos[idx + 1] = s1;
			mVertexSkinInfos[idx + 2] = s2;
		}

		for (int i = 0 ; i < geo0->numskelbone ; i++)
		{
			OlaASE::skeletonbone* sb= geo0->skelbones + i;

			tSBone* sbone = new tSBone();
			sbone->id_ase = sb->id;
			sbone->name = olastring(sb->name);
			mSkeletonBones.push_back(sbone);
		}
	}

	for (int i = 0 ; i < num_vert ; i++)
	{
		mMinVertex[0] = vert_data[i].pos[0] < mMinVertex[0] ? vert_data[i].pos[0] : mMinVertex[0];
		mMinVertex[1] = vert_data[i].pos[1] < mMinVertex[1] ? vert_data[i].pos[1] : mMinVertex[1];
		mMinVertex[2] = vert_data[i].pos[2] < mMinVertex[2] ? vert_data[i].pos[2] : mMinVertex[2];

		mMaxVertex[0] = vert_data[i].pos[0] > mMaxVertex[0] ? vert_data[i].pos[0] : mMaxVertex[0];
		mMaxVertex[1] = vert_data[i].pos[1] > mMaxVertex[1] ? vert_data[i].pos[1] : mMaxVertex[1];
		mMaxVertex[2] = vert_data[i].pos[2] > mMaxVertex[2] ? vert_data[i].pos[2] : mMaxVertex[2];
	}

	return true;
}

void OlaSubMesh::setOrgMaterial( OlaMaterial* mat )
{
	if (mOrgMaterial)	
		mOrgMaterial->delRef();	

	mOrgMaterial = mat;
	mOrgMaterial->addRef();
}

//bool OlaSubMesh::attachSkeleton(OlaSkeleton* skel)
//{
//	int num_sbone = mSkeletonBones.size();
//	for (int i = 0 ; i < num_sbone ; i++)
//	{
//		tSBone& sbone = mSkeletonBones[i];
//		
//		OlaBone* bone = skel->bone(sbone.name);
//		assert( bone && "bones not match !");
//	}
//
//	mSkeleton = skel;
//
//	return true;
//}

