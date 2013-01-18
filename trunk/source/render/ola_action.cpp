#include "ola_action.h"

#include "ola_action_res.h"
#include "ola_skeleton.h"
#include "ola_animation.h"

#include "math/ola_math.h"

#include "ola_resourcemng.h"

OlaBoneState::OlaBoneState():
bone(0)
{
	pos_r.Set(0,0,0);
	rot_r.Set(0,0,0,1.0f);
	matrix_cur.Identity();
	matrix_inv.Identity();
}

OlaAction::OlaAction():
mResource(0),
mSkeleton(0),
mPassedTime(0)
{

}

OlaAction::~OlaAction()
{
	clear();
}

void OlaAction::clear()
{
	for (size_t i = 0 ; i < mAnimationStates.size() ; i++)
	{
		delete mAnimationStates[i];
	}
	mAnimationStates.clear();

	mSkeleton = 0;
	mResource = 0;	
}

void OlaAction::init(OlaSkeleton* skeleton,OlaActionResource* resouce)
{
	clear();

	mSkeleton = skeleton;
	mResource = resouce;

	int bone_num = mSkeleton->bones().size();

	mAnimationStates.resize(bone_num,0);

	for (size_t i = 0 ; i < mSkeleton->bones().size() ; i++)
	{
		OlaBone* bone = mSkeleton->bones()[i];
		
		OlaAnimationTrack* ani_track = mResource->track(bone->index);

		assert(ani_track);

		mAnimationStates[bone->index] = new OlaAnimationState();
	}
	
	mTotalTime = mResource->totalTime();
}

void OlaAction::setPassTime(float time)
{
	if(mTotalTime == 0.0f)
	{

	}
	else
	{			
		mPassedTime = time;

		if (mPassedTime > mTotalTime)
		{
			mPassedTime = mTotalTime;
		}

		int bone_num = mSkeleton->bones().size();
		for (size_t i = 0 ; i < bone_num; i++)
		{
			OlaBone* bone = mSkeleton->bones().at(i);
			assert(bone);
			
			OlaAnimationTrack* ani_track = mResource->track(bone->name.c_str());
			assert(ani_track);

			OlaAnimationState* ani_state = mAnimationStates.at(bone->index);
			assert(ani_state);

			ani_track->calculateAnimationFrame(mPassedTime,bone,ani_state);

			ani_state->currentMat().Compose(ani_state->currentRot(),ani_state->currentPos());
		}
	}
		
}

void OlaAction::resetPassTime()
{
	mPassedTime = 0;
}

const char* OlaAction::name()
{
	return mResource->filename().c_str();
}


OlaActionChannel::OlaActionChannel(int id):
mWeight(1.0f),
mSkeleton(0),
mCurrentAction(0),
mLoop(false),
mNextAction(0),
mIndex(id)
{

}

OlaActionChannel::~OlaActionChannel()
{
	_clearActions();
}

void OlaActionChannel::initialize( OlaSkeleton* skeleton )
{
	_clearActions();
	mSkeleton = skeleton;
	mCurrentAction = 0;
	mNextAction = 0;
}

void OlaActionChannel::setActionSeq( const char** action_names,int count,bool loop )
{
	_clearActions();

	for (size_t i = 0 ; i < count ; i++)
	{
		const char* action_name = action_names[i];

		OlaAction* action = new OlaAction();
		OlaActionResource* action_res = OlaResourceMng::instance()->getActionResource(action_name);

		assert( mSkeleton && " no animation channel skeleton !");
		assert( action_res && "no animation resource !");

		action->init( mSkeleton , action_res );
		mActions.push_back(action);
	}

	mLoop = loop;
}

int OlaActionChannel::setAction( const char* action_name,bool loop )
{
	assert( mSkeleton && " no animation channel skeleton !");

	mLoop = loop;
	for (size_t i = 0 ; i < mActions.size() ;i++)
	{
		if (strcmp(mActions[i]->name(),action_name) == 0)
		{
			mCurrentAction = mActions[i];

			if (i == mActions.size() - 1 && mLoop)
			{
				mNextAction = mActions[0];
			}
			else
				mNextAction = mCurrentAction;

			return i;
		}
	}

	OlaAction* action = new OlaAction();
	OlaActionResource* action_res = OlaResourceMng::instance()->getActionResource(action_name);

	assert( action_res && "no animation resource !");
	action->init(mSkeleton,action_res);

	if (mActions.size() != 0)
	{
		delete mActions[0];
		mActions[0] = action;		 
	}
	else
		mActions.push_back(action);

	mCurrentAction = mActions[0];

	if (mActions.size() == 1)
	{
		mNextAction = mCurrentAction;
	}
	else
		mNextAction = mActions[1];
	
	return 0;
}

int OlaActionChannel::setNextAction( const char* action_name,bool loop )
{
	mLoop = loop;
	return 1;
}

void OlaActionChannel::_clearActions()
{
	for (size_t i = 0 ; i < mActions.size() ;i++)
	{
		delete mActions[i];
	}
	mActions.clear();
}

void OlaActionChannel::updateAnmSingle( float elasped )
{
	if (mCurrentAction)
	{
		mCurrentAction->setPassTime(elasped);

		float passed_time = mCurrentAction->passedTime();
		float total_time = mCurrentAction->totalTime();
		if (passed_time > total_time)
		{
			mCurrentAction->resetPassTime();
		}					
	}
}

void OlaActionChannel::updateAnmSeq( float elasped )
{

}

void OlaActionChannel::setPassedTime( float passed_time )
{
	if (mCurrentAction)
	{
		mCurrentAction->setPassTime(passed_time);				
	}
}

OlaActionChannelSet::OlaActionChannelSet()
{

}

OlaActionChannelSet::~OlaActionChannelSet()
{
	for (size_t i = 0 ; i < mBoneStates.size() ; i++)
	{
		delete mBoneStates[i];
	}
	mBoneStates.clear();
}

void OlaActionChannelSet::initialize( OlaSkeleton* skeleton )
{
	mSkeleton = skeleton;
	int bone_num = mSkeleton->bones().size();
	mBoneStates.resize(bone_num,0);
	for (size_t i = 0 ; i < mSkeleton->bones().size() ; i++)
	{
		OlaBone* bone = mSkeleton->bones()[i];
		
		OlaBoneState* bone_state = new OlaBoneState();
		bone_state->bone = bone;
		bone_state->matrix_cur.Identity();
		bone_state->matrix_inv.Identity();

		mBoneStates[bone->index] = bone_state;
	}

	createChannel();
}

int OlaActionChannelSet::createChannel()
{
	int id = mChannels.size();
	OlaActionChannel* channel = new OlaActionChannel(id);

	assert( mSkeleton && "no animation channel skeleton !");

	channel->initialize(mSkeleton);
	mChannels.push_back(channel);
	return id;
}

static olaMat4 temp;
void OlaActionChannelSet::update( float elasped )
{
	//mChannels[0]->updateAnmSingle(elasped);

	//OlaAction* cur_action = mChannels[0]->currentAction();

	//if (cur_action)
	//{
	//	OlaBone::BoneList& bones = mSkeleton->bones();
	//	int bone_num = (int)bones.size();
	//	
	//	for (int i = 0 ; i < bone_num ; i++)
	//	{
	//		OlaBone* bone = bones[i];
	//		
	//		OlaBoneState* bone_state = mBoneStates[bone->index];
	//		OlaAnimationState* ani_state = cur_action->animState(bone->index);

	//		bone_state->pos_r = ani_state->currentPos();
	//		bone_state->rot_r = ani_state->currentRot();
	//	}
	//}

	//OlaBone* root = mSkeleton->root();
	//olaMat4 m;m.Identity();
	//_updateBoneStateMatrix(root,m);
}

void OlaActionChannelSet::_updateBoneStateMatrix( OlaBone* bone,olaMat4& parent_mat )
{
	OlaBoneState* bone_state = mBoneStates[bone->index];
	
	temp.Compose(bone_state->rot_r,bone_state->pos_r);

	bone_state->matrix_cur.Multiply(parent_mat,temp);

	bone_state->matrix_inv.Multiply(bone_state->matrix_cur,bone->matrix_iorg);

	int sub_num = (int)bone->children.size();

	for(int i = 0 ; i < sub_num ; i++)
	{
		_updateBoneStateMatrix(bone->children[i],bone_state->matrix_cur);
	}
}

void OlaActionChannelSet::setPassedTime( float time )
{
	mChannels[0]->setPassedTime(time);

	OlaAction* cur_action = mChannels[0]->currentAction();

	if (cur_action)
	{
		OlaBone::BoneList& bones = mSkeleton->bones();
		int bone_num = (int)bones.size();
		
		for (int i = 0 ; i < bone_num ; i++)
		{
			OlaBone* bone = bones[i];
			
			OlaBoneState* bone_state = mBoneStates[bone->index];
			OlaAnimationState* ani_state = cur_action->animState(bone->index);

			bone_state->pos_r = ani_state->currentPos();
			bone_state->rot_r = ani_state->currentRot();
		}
	}

	OlaBone* root = mSkeleton->root();
	olaMat4 m;m.Identity();
	_updateBoneStateMatrix(root,m);
}
