#include "ola_ase.h"
#include "ola_assetmng.h"

#include "ola_animation.h"
#include "ola_action_res.h"
#include "ola_action.h"

OlaActionResource::OlaActionResource(const char* name):
mFilename(name)
{

}

OlaActionResource::~OlaActionResource()
{
	for (size_t i = 0 ; i < mAnimationTracks.size() ; i ++)
	{
		delete mAnimationTracks[i];
	}
	mAnimationTracks.clear();
}

bool OlaActionResource::load(OlaAsset* asset)
{
	OlaASE::model* model = OlaASE::load(asset->data,asset->length,1.0f);
	
	mClipNum = model->lastframe - model->firstframe;
	mFrameSpeed = model->framespeed; //clips per second
	mTickPerFrame = model->tickperframe;

	mTotalTime = (float)mClipNum / (float)mFrameSpeed;
	mClipTime = mTotalTime / (float)mClipNum;

	for (size_t i = 0 ; i < model->geom.size() ; i++)
	{
		_loadOneTrack(model->geom[i]);
	}

	delete model;

	return true;
}

bool OlaActionResource::_loadOneTrack(OlaASE::geomobj* geom)
{
	olastring track_name(geom->name);	

	OlaASE::animdata* anim = geom->anim;

	float frame_interval = 1.0f / mFrameSpeed;

	OlaAnimationTrack* track = new OlaAnimationTrack(track_name.c_str(),mTotalTime,mClipNum, mClipTime );

	mAnimationTracks.push_back(track);

	if (!anim)
	{
		return true;
	}

	if(anim->numpos == 0)
	{
		//set static 
	}
	else
	{
		int first_frametime = anim->posframetime[0];

		track->mPosKeys.resize(anim->numpos);
		for (int i = 0 ; i < anim->numpos ; i++)
		{
			int idx = i * 3;
			float* pos = anim->pos + idx;
			track->mPosKeys[i].pos.Set(pos[0],pos[1],pos[2]);
			track->mPosKeys[i].frametime = (float)(anim->posframetime[i] - first_frametime) / mTickPerFrame * mClipTime; //ticks
		}

		track->mPosKeyIndexs.resize(mClipNum);
		for (int c_i = 0 ; c_i < mClipNum ; c_i++)
		{
			float clip_time_s = c_i * mClipTime;

			for (int i = 0 ; i < anim->numpos ; i++)
			{
				if(track->mPosKeys[i].frametime > clip_time_s ||
					i == anim->numrot - 1)
				{
					track->mPosKeyIndexs[c_i].e_idx = i;
					track->mPosKeyIndexs[c_i].s_idx = i - 1;
					break;
				}
			}
		}
	}

	if(anim->numrot == 0)
	{
		//set static 
	}
	else
	{
		int first_frametime = anim->rotframetime[0];

		track->mRotKeys.resize(anim->numrot);
		olaQuat pre_q,p,q;

		p.Set(0.0f,0.0f,0.0f,1.0f);
		q.Set(0.0f,0.0f,0.0f,1.0f);
		pre_q.Set(0.0f,0.0f,0.0f,1.0f);
		
		for (int i = 0 ; i < anim->numrot ; i++)
		{
			int idx = i * 4;
			float* rot = anim->rot + idx;

			p.FromAxisAngle(rot[0],rot[1],rot[2],rot[3] * olaMath::M_RAD2DEG);
			p.Normalize();			
			q = p * pre_q;
			q.Normalize();

			track->mRotKeys[i].rot = q;
			track->mRotKeys[i].frametime = (float)(anim->rotframetime[i] - first_frametime) / mTickPerFrame * mClipTime; //ticks

			pre_q = q;
		}

		track->mRotKeyIndexs.resize(mClipNum);
		for (int c_i = 0 ; c_i < mClipNum ; c_i++)
		{
			float clip_time_s = c_i * mClipTime;

			for (int i = 0 ; i < anim->numrot ; i++)
			{
				if(track->mRotKeys[i].frametime > clip_time_s ||
				   i == anim->numrot - 1)
				{
					track->mRotKeyIndexs[c_i].e_idx = i;
					track->mRotKeyIndexs[c_i].s_idx = i - 1;
					break;
				}
			}
		}
	}	

	return true;
}

OlaAnimationTrack* OlaActionResource::track( const char* name )
{
	for (int i = 0 ; i < mAnimationTracks.size() ; i++)
	{
		OlaAnimationTrack* track = mAnimationTracks[i];
		if(!strcmp(name,track->name().c_str()))
			return track;
	}
	return 0;
}

