#ifndef __OLA__SKELETON_MODEL__H__
#define __OLA__SKELETON_MODEL__H__

#include "ola_string.h"

class OlaAction;
class OlaMesh;
class OlaSkin;
class OlaSkeleton;
class OlaResourceMng;
class OlaActionChannel;
class OlaActionChannelSet;
struct OlaBoneState;

class OlaSkeletonModel
{
public:
	OlaSkeletonModel(OlaSkeleton* skel,OlaResourceMng* res_mng);

	virtual ~OlaSkeletonModel();

	void setPassedTime(float t);

	void update(float elasped);

	void setCpuSkin(OlaMesh* skin_res);

	bool setAction(const char* act_name);

	OlaSkin* skin(){return mCPUSkin;}

	OlaAction* currentAction(int channel);

	OlaSkeleton* skeleton(){return mSkeleton;}

	OlaBoneState* boneState(int index);
protected:

	void _updateCPUSkin();

	olastring mCurrentActionName;
	OlaSkeleton* mSkeleton;
	OlaSkin* mCPUSkin;
	OlaActionChannelSet* mChannelset;
	OlaResourceMng* mResourceMng;
};

#endif