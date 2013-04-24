#ifndef _OLA_MESH_H__
#define _OLA_MESH_H__

#include "ola_stl.h"

#include "ola_assetmng.h"
#include "ola_geobuffer.h"

#include "ola_ase.h"
#include "ola_string.h"
#include "ola_resource.h"

class OlaMaterial;
class OlaSubMesh;
class OlaRenderOp;
class OlaSkeleton;

class OlaMesh : public IRefCounter
{
public:
	typedef OlaArray<OlaSubMesh*> SubMeshList;
public:
	OlaMesh(const char* name);

	~OlaMesh();

	void clear();

	bool copyTo(OlaMesh* mesh,OlaVBParam::VBO_TYPE vb_type);

	olastring& name() {return mName;}

	SubMeshList& submeshs() {return mSubMeshs;}

protected:
	olastring mName;
	SubMeshList mSubMeshs;
};

class OlaSubMesh //处理一个ase中多个材质的问题 所以需要有mesh-submesh机制
{
public:

	struct tVSkinInfo
	{
		tVSkinInfo(){memset(bone_id,-1,sizeof(bone_id));memset(weight,0,sizeof(weight));}
		int bone_id[4];
		float weight[4];
	};

	struct tSBone
	{
		int id_ase;
		int id_skl;
		olastring name;
	};

	typedef OlaArray<tSBone*> SBoneList;
	typedef OlaArray<tVSkinInfo> VSkinInfoList;

	typedef OlaArray<int> MeshFaceList;
public:
	OlaSubMesh(const char* name);
	virtual ~OlaSubMesh();

	virtual void clear();

	virtual void copyTo(OlaSubMesh* submesh,OlaVBParam::VBO_TYPE vb_type);

	VSkinInfoList& vskinInfo(){return mVertexSkinInfos;}
	SBoneList& boneList(){return mSkeletonBones;}

	olastring& name(){return mName;}
	OlaMaterial* material() {return mMaterial;}
	void setMaterial(OlaMaterial* mat);

	virtual int getElementOffset(OlaVBParam::VB_ELEMENT_TYPE type){return mVB->elementOffset(type);}

	OlaVB* vb(){return mVB;}
	OlaIB* ib(){return mIB;}

	inline float* minv(){return mMinVertex;}
	inline float* maxv(){return mMaxVertex;}
	
protected:
	olastring mName;

	OlaMaterial* mMaterial; //一个子网格就一个材质了 不会那么变态

	OlaVB* mVB;
	OlaIB* mIB;

	VSkinInfoList mVertexSkinInfos;
	SBoneList mSkeletonBones;	

	float mMinVertex[3];
	float mMaxVertex[3];
};

#endif
