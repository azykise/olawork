#ifndef _OLA_ANIMATION_H__
#define _OLA_ANIMATION_H__

#include <map>
#include <vector>

#include "math/ola_math.h"
#include "ola_util.h"
#include "ola_ase.h"

struct OlaPosKey
{
	OlaPosKey()
	{
		pos.Zero();
		frametime = 0;
	}

	float frametime; //second
	olaVec3 pos;
};

struct OlaRotKey
{
	OlaRotKey()
	{
		rot.Set(0.0f,0.0f,0.0f,1.0f);
	}

	float frametime; //second
	olaQuat rot;
};

struct OlaClipIndex
{
	OlaClipIndex():
	s_idx(0),e_idx(0){}
	int s_idx;
	int e_idx;
};

class OlaAnimationState;
class OlaActionResource;
struct OlaBone;
//!动画轨 记录一个动画节点的所有关键帧数据 并且负责根据时间戳插值计算
class OlaAnimationTrack
{
public:
	OlaAnimationTrack(const char* name,float totaltime,int clipnum,float cliptime);
	~OlaAnimationTrack();
	//!根据轨道时间 计算出一帧的动画数据
	void calculateAnimationFrame(float track_time,OlaBone* bone,OlaAnimationState* ani_state);

	inline olastring& name(){return mName;}

	inline float totalTime(){return mTotalTime;}
	
	inline float frameInterval(){return mClipTime;}

	inline int numPosKeys(){return (int)mPosKeys.size();}
	inline int numRotKeys(){return (int)mRotKeys.size();}

protected:
	friend OlaActionResource;
	//!关键帧索引 长度为 framenum , 用来指示每一个时间戳上对应的关键帧
	std::vector<OlaClipIndex> mPosKeyIndexs;
	std::vector<OlaClipIndex> mRotKeyIndexs;

	std::vector<OlaPosKey> mPosKeys;
	std::vector<OlaRotKey> mRotKeys;
	
	olaVec3 mStaticPos;
	olaQuat mStaticRot;

	//!动画时间 秒
	float mTotalTime; 

	//!逻辑帧数
	int mClipNum;

	//!帧间隔 = mTotalTiem / mClipNum
	float mClipTime;

	olastring mName;	
};

//!动画状态 一个动画节点的动画实例 负责维护当前动画的状态
class OlaAnimationState
{
public:
	OlaAnimationState();
	virtual ~OlaAnimationState();

	void reset();

	inline olaVec3& currentPos(){return mCurrentPos;}
	inline olaQuat& currentRot(){return mCurrentRot;}

	inline olaMat4& currentMat(){return mCurrentMat;}
protected:

	olaVec3 mCurrentPos;
	olaQuat mCurrentRot;	

	olaMat4 mCurrentMat;
};

#endif