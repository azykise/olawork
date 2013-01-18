#ifndef _OLA_ACTION_RES_INCLUDE_H__
#define _OLA_ACTION_RES_INCLUDE_H__

#include <map>
#include <vector>

#include "math/ola_math.h"
#include "ola_util.h"
#include "ola_ase.h"

class OlaAnimationTrack;
class OlaAsset;
//!只是作为一个有名字的AnimationTrack集合 应该对应一个动作ase文件
class OlaActionResource
{
	typedef std::vector<OlaAnimationTrack*> AnimationTrackList;
public:
	OlaActionResource(const char* filename);
	~OlaActionResource();

	bool load(OlaAsset* asset);

	inline int trackCount(){return (int)mAnimationTracks.size();}

	OlaAnimationTrack* track(int index){return mAnimationTracks[index];}
	OlaAnimationTrack* track(const char* name);

	inline olastring& filename(){return mFilename;}

	inline float totalTime(){return mTotalTime;}
	inline int clipNum(){return mClipNum;}
protected:

	bool _loadOneTrack(OlaASE::geomobj* geom);

	float mTotalTime;
	float mClipTime; // totaltime / clipnum

	int mTickPerFrame;
	int mFrameSpeed; //clips per second
	int mClipNum;

	AnimationTrackList mAnimationTracks;

	olastring mFilename;
};


#endif