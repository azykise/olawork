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
//!������ ��¼һ�������ڵ�����йؼ�֡���� ���Ҹ������ʱ�����ֵ����
class OlaAnimationTrack
{
public:
	OlaAnimationTrack(const char* name,float totaltime,int clipnum,float cliptime);
	~OlaAnimationTrack();
	//!���ݹ��ʱ�� �����һ֡�Ķ�������
	void calculateAnimationFrame(float track_time,OlaBone* bone,OlaAnimationState* ani_state);

	inline olastring& name(){return mName;}

	inline float totalTime(){return mTotalTime;}
	
	inline float frameInterval(){return mClipTime;}

	inline int numPosKeys(){return (int)mPosKeys.size();}
	inline int numRotKeys(){return (int)mRotKeys.size();}

protected:
	friend OlaActionResource;
	//!�ؼ�֡���� ����Ϊ framenum , ����ָʾÿһ��ʱ����϶�Ӧ�Ĺؼ�֡
	std::vector<OlaClipIndex> mPosKeyIndexs;
	std::vector<OlaClipIndex> mRotKeyIndexs;

	std::vector<OlaPosKey> mPosKeys;
	std::vector<OlaRotKey> mRotKeys;
	
	olaVec3 mStaticPos;
	olaQuat mStaticRot;

	//!����ʱ�� ��
	float mTotalTime; 

	//!�߼�֡��
	int mClipNum;

	//!֡��� = mTotalTiem / mClipNum
	float mClipTime;

	olastring mName;	
};

//!����״̬ һ�������ڵ�Ķ���ʵ�� ����ά����ǰ������״̬
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