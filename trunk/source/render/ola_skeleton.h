#ifndef _OLA_SKELETON_H__
#define _OLA_SKELETON_H__

#include <vector>

#include "math/ola_math.h"
#include "ola_util.h"

struct OlaBone
{
	typedef std::vector<OlaBone*> BoneList;
	OlaBone();
	~OlaBone();
	//!骨骼名
	olastring name;
	//!骨骼在骨架中的索引
	int index;
	//!父骨骼
	OlaBone* parent;
	//!子骨骼
	BoneList children;
	//!初始状态(fig状态)时相对于父骨骼的位移
	olaVec3 pos_r;
	//!初始状态(fig状态)时相对于父骨骼的旋转
	olaQuat rot_r;
	//!在根骨骼坐标系下骨骼的初始状态矩阵
	olaMat4	matrix_org;
	//!matrix_org的逆
	olaMat4	matrix_iorg;
};

class OlaAsset;
class OlaSkeleton
{
public:
	OlaSkeleton(const char* name);
	~OlaSkeleton();

	bool load(OlaAsset* asset);

	OlaBone* bone(olastring& name);

	OlaBone* root(){return mRoot;}

	OlaBone::BoneList& bones(){return mBones;}

protected:

	void _setBoneRelativeMatrix(OlaBone* bone);

	//!骨架名
	olastring mName;
	//!根骨骼
	OlaBone*	mRoot;
	//!根骨骼ID
	int			mRootID;
	//!所有骨骼
	OlaBone::BoneList mBones;	
};

#endif