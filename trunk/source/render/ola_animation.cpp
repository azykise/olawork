#include "ola_animation.h"

#include "ola_skeleton.h"

OlaAnimationTrack::OlaAnimationTrack(const char* name,float totaltime,int clipnum,float cliptime):
mName(name),
mTotalTime(totaltime),
mClipNum(clipnum),
mClipTime(cliptime)
{
	mStaticPos.Set(0.0f,0.0f,0.0f);
	mStaticRot.Set(0.0f,0.0f,0.0f,1.0f);

}

OlaAnimationTrack::~OlaAnimationTrack()
{

}

void OlaAnimationTrack::calculateAnimationFrame(float track_time,OlaBone* bone,OlaAnimationState* ani_state)
{
	float f = track_time / mClipTime;
	int current_clip = (int)(f);

	if(mPosKeyIndexs.size() == 0)
	{
		ani_state->currentPos().Set(bone->pos_r.x,bone->pos_r.y,bone->pos_r.z);
	}
	else
	{
		OlaClipIndex& poskey_idx = mPosKeyIndexs[current_clip];
		
		float dt_pos = mPosKeys[poskey_idx.e_idx].frametime - mPosKeys[poskey_idx.s_idx].frametime;
		float ct_pos = track_time - mPosKeys[poskey_idx.s_idx].frametime;

		float inter_pos = dt_pos ? ct_pos / dt_pos : 1.0f;
		float inter_r = 1.0f - inter_pos;

		float new_x = mPosKeys[poskey_idx.s_idx].pos.x * inter_r + mPosKeys[poskey_idx.e_idx].pos.x * inter_pos;
		float new_y = mPosKeys[poskey_idx.s_idx].pos.y * inter_r + mPosKeys[poskey_idx.e_idx].pos.y * inter_pos;
		float new_z = mPosKeys[poskey_idx.s_idx].pos.z * inter_r + mPosKeys[poskey_idx.e_idx].pos.z * inter_pos;

		ani_state->currentPos().Set(new_x ,new_y , new_z );
	}

	if(mRotKeyIndexs.size() == 0)
	{
		ani_state->currentRot().Set(bone->rot_r.x,bone->rot_r.y,bone->rot_r.z,bone->rot_r.w);
	}
	else
	{
		OlaClipIndex& rotkey_idx = mRotKeyIndexs[current_clip];

		float dt_rot = mRotKeys[rotkey_idx.e_idx].frametime - mRotKeys[rotkey_idx.s_idx].frametime;
		float ct_rot = track_time - mRotKeys[rotkey_idx.s_idx].frametime;

		float inter_rot = dt_rot ? ct_rot / dt_rot : 0.0f;
		
		olaQuat& q1 = mRotKeys[rotkey_idx.s_idx].rot;
		olaQuat& q2 = mRotKeys[rotkey_idx.e_idx].rot;

		ani_state->currentRot().Slerp(q1,q2,1.0f - inter_rot);
	}
}

OlaAnimationState::OlaAnimationState()
{
	reset();
}

OlaAnimationState::~OlaAnimationState()
{

}

void OlaAnimationState::reset()
{
	mCurrentPos.Set(0.0f,0.0f,0.0f);
	mCurrentRot.Set(0.0f,0.0f,0.0f,1.0f);

	mCurrentMat.Identity();
}