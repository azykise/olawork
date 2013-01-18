#ifndef _OLA_ACTION_H__
#define _OLA_ACTION_H__

#include <vector>

#include "math/ola_math.h"
#include "ola_util.h"

class OlaAnimationTrack;
class OlaAnimationState;
class OlaSkeleton;

//!����������״̬ �����Ͷ��AnimationState�ں϶���
struct OlaBone;
struct OlaBoneState
{
	OlaBoneState();
	~OlaBoneState(){}

	OlaBone* bone;

	//!��ǰ,�����ڸ������ռ��µ�λ��
	olaVec3 pos_r;
	//!��ǰ,�����ڸ������ռ��µ���ת
	olaQuat rot_r;
	//!��ǰ,�����ڹ�������ϵ�µľ���
	olaMat4 matrix_cur; //matrix_cur = parent_bonestate->matrix_cur * bone_state.matrix_interval
	//!��ǰ,��������ڹ�������ϵԭ��ı任�����
	olaMat4 matrix_inv; //matrix_inv = matrix_cur * bone->matrix_iorg
};

class OlaActionResource;
//!���� ����һ����ɫ�������ж����ڵ�һ��������ȫ��������Ϣ
//!����ԭʼ������Ϣ ��ǰ������Ϣ
class OlaAction
{
	typedef std::vector<OlaAnimationState*> AnimationStateList;
public:
	OlaAction();
	virtual ~OlaAction();

	const char* name();
	void init(OlaSkeleton* skeleton,OlaActionResource* resouce);
	void clear();

	void setPassTime(float passed_time);
	void resetPassTime();

	inline float passedTime(){return mPassedTime;}
	inline float totalTime() {return mTotalTime;}

	inline OlaAnimationState* animState(int bone_id){return mAnimationStates[bone_id];};
	inline OlaActionResource* actionResource(){return mResource;}

protected:

	AnimationStateList mAnimationStates; //���й����ĵ�ǰ����״̬

	OlaSkeleton* mSkeleton;
	OlaActionResource* mResource;

	float mPassedTime;
	float mTotalTime; //second
};

//!������ �������������ɵ����� ����֮����Թ���
class OlaActionChannel
{
	typedef std::vector<OlaAction*> ActionList;
public:
	OlaActionChannel(int id);
	virtual ~OlaActionChannel();
	
	void initialize(OlaSkeleton* skeleton);	

	//!���ö�������
	void setActionSeq(const char** action_names,int count,bool loop);
	//!���õ�ǰҪ���ŵĶ���
	int setAction(const char* action_name,bool loop);
	//!������һ��Ҫ���ŵĶ���
	int setNextAction(const char* action_name,bool loop);

	void setPassedTime(float passed_time);

	void updateAnmSingle(float elasped);
	void updateAnmSeq(float elasped);

	inline OlaAction* currentAction() {return mCurrentAction;}
protected:

	void _clearActions();

	
	OlaAction* mCurrentAction;
	OlaAction* mNextAction;

	ActionList mActions;
	OlaSkeleton* mSkeleton;

	float mWeight;
	bool mLoop;
	int mIndex;
};

//!�����켯�� ��������� ������֮������ں�
class OlaActionChannelSet
{
	typedef std::vector<OlaBoneState*> BoneStateList;
	typedef std::vector<OlaActionChannel*> ChannelList;
public:	
	OlaActionChannelSet();
	virtual ~OlaActionChannelSet();

	void initialize(OlaSkeleton* skeleton);
	
	void update(float elasped);

	void setPassedTime(float time);

	int createChannel();
	int removeChannel(){return -1;}
	int resetChannelID(int oldid,int newid){return -1;}

	inline OlaActionChannel* channel(int idx){return mChannels[idx];}

	inline int channelNum(){return (int)(mChannels.size());}

	inline OlaBoneState* boneState(int bone_id){return mBoneStates[bone_id];}

protected:

	void _updateBoneStateMatrix(OlaBone* bone,olaMat4& parent_mat);

	ChannelList mChannels;

	BoneStateList mBoneStates;

	OlaSkeleton* mSkeleton;
};

#endif