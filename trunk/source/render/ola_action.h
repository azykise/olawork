#ifndef _OLA_ACTION_H__
#define _OLA_ACTION_H__

#include <vector>

#include "math/ola_math.h"
#include "ola_util.h"

class OlaAnimationTrack;
class OlaAnimationState;
class OlaSkeleton;

//!骨骼的最终状态 可能油多个AnimationState融合而来
struct OlaBone;
struct OlaBoneState
{
	OlaBoneState();
	~OlaBoneState(){}

	OlaBone* bone;

	//!当前,骨骼在父骨骼空间下的位移
	olaVec3 pos_r;
	//!当前,骨骼在父骨骼空间下的旋转
	olaQuat rot_r;
	//!当前,骨骼在骨骼坐标系下的矩阵
	olaMat4 matrix_cur; //matrix_cur = parent_bonestate->matrix_cur * bone_state.matrix_interval
	//!当前,骨骼相对于骨骼坐标系原点的变换逆矩阵
	olaMat4 matrix_inv; //matrix_inv = matrix_cur * bone->matrix_iorg
};

class OlaActionResource;
//!动作 包含一个角色身上所有动画节点一个动作的全部动画信息
//!包括原始动画信息 当前动画信息
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

	AnimationStateList mAnimationStates; //所有骨骼的当前动画状态

	OlaSkeleton* mSkeleton;
	OlaActionResource* mResource;

	float mPassedTime;
	float mTotalTime; //second
};

//!动作轨 包含多个动作组成的序列 动作之间可以过渡
class OlaActionChannel
{
	typedef std::vector<OlaAction*> ActionList;
public:
	OlaActionChannel(int id);
	virtual ~OlaActionChannel();
	
	void initialize(OlaSkeleton* skeleton);	

	//!设置动作序列
	void setActionSeq(const char** action_names,int count,bool loop);
	//!设置当前要播放的动作
	int setAction(const char* action_name,bool loop);
	//!设置下一个要播放的动作
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

//!动作轨集合 多个动作轨 动作轨之间可以融合
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