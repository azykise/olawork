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
	//!������
	olastring name;
	//!�����ڹǼ��е�����
	int index;
	//!������
	OlaBone* parent;
	//!�ӹ���
	BoneList children;
	//!��ʼ״̬(fig״̬)ʱ����ڸ�������λ��
	olaVec3 pos_r;
	//!��ʼ״̬(fig״̬)ʱ����ڸ���������ת
	olaQuat rot_r;
	//!�ڸ���������ϵ�¹����ĳ�ʼ״̬����
	olaMat4	matrix_org;
	//!matrix_org����
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

	//!�Ǽ���
	olastring mName;
	//!������
	OlaBone*	mRoot;
	//!������ID
	int			mRootID;
	//!���й���
	OlaBone::BoneList mBones;	
};

#endif