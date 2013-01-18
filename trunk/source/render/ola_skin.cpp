#include "math/ola_math.h"

#include "ola_skin.h"

#include "ola_mesh.h"
#include "ola_resourcemng.h"

#include "ola_skeleton.h"
#include "ola_action.h"

OlaSkin::OlaSkin( OlaMesh* org_mesh ):
mSrcMesh(org_mesh)
{
	mSkinMesh = new OlaMesh("");
	memset(mBoneIDTable,0,sizeof(int) * 128);
}

OlaSkin::~OlaSkin()
{
	delete mSkinMesh;
}

bool OlaSkin::attachSkeleton( OlaSkeleton* skeleton )
{
	mSkeleton = skeleton;

	mSrcMesh->copyTo(mSkinMesh,OlaVBParam::VBO_DYNAMIC);	

	OlaMesh::SubMeshList& sub_meshs = mSkinMesh->submeshs();
	OlaMesh::SubMeshList::iterator i = sub_meshs.begin();
	while( i != sub_meshs.end())
	{
		OlaSubMesh* sub_mesh = *i;

		OlaSubMesh::SBoneList& sbones = sub_mesh->boneList();
		OlaSubMesh::SBoneList::iterator b_i = sbones.begin();
		while(b_i != sbones.end())
		{
			OlaSubMesh::tSBone* sbone = *b_i;

			OlaBone* bone = mSkeleton->bone(sbone->name);

			sbone->id_skl = bone->index;

			b_i ++;
		}

		i++;
	}

	return true;
}

struct tOlaVertex
{
	float pos[3];
	float nor[3];
	float uv0[2];
};

void OlaSkin::updateCPUSkin( OlaActionChannelSet* channel_set )
{
	OlaMesh::SubMeshList& org_meshs = mSrcMesh->submeshs();
	OlaMesh::SubMeshList& sub_meshs = mSkinMesh->submeshs();

	std::vector<OlaBone*> bones;
	std::vector<float> weights;

	for (size_t mi = 0 ; mi < sub_meshs.size() ; mi++)
	{
		OlaSubMesh* org_submesh = org_meshs[mi];

		OlaSubMesh* sub_mesh = sub_meshs[mi];
		
		OlaVB* vb_org = org_submesh->vb();
		OlaVB* vb = sub_mesh->vb();

		int num_vert = vb->num_v();

		tOlaVertex* vert_data = (tOlaVertex*)(vb->lock());
		tOlaVertex* vert_dataorg = (tOlaVertex*)(vb_org->localData());

		for (int i = 0 ; i < num_vert ; i++)
		{
			bones.clear();
			weights.clear();
			OlaSubMesh::tVSkinInfo& info = sub_mesh->vskinInfo()[i];

			for (int bi = 0 ; bi != 4 ; bi++)
			{
				int bone_id = info.bone_id[bi];
				if (bone_id != -1)
				{
					OlaSubMesh::tSBone* sbone = sub_mesh->boneList()[bone_id];
					OlaBone* bone = mSkeleton->bones()[sbone->id_skl];
					
					bones.push_back(bone);
					weights.push_back(info.weight[bi]);
				}
			}

			olaVec3 temp(0,0,0);

			assert( bones.size() && "skin error !");

			for (size_t bi = 0 ; bi < bones.size() ; bi++)
			{
				OlaBone* bone = bones[bi];
				float w = weights[bi];

				OlaBoneState* bone_state = channel_set->boneState(bone->index);

				olaVec3 pos(vert_dataorg[i].pos[0],vert_dataorg[i].pos[1],vert_dataorg[i].pos[2]);

				olaVec3 pos_new = bone_state->matrix_inv * pos;
					
				temp = temp + pos_new * w;
			}

			vert_data[i].pos[0] = temp.x;
			vert_data[i].pos[1] = temp.y;
			vert_data[i].pos[2] = temp.z;			
		}

		vb->unlock();
	}

}
