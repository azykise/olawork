#include "ola_sklmodel.h"


#include "ola_resourcemng.h"
#include "ola_action.h"
#include "ola_mesh.h"
#include "ola_skin.h"

OlaSkeletonModel::OlaSkeletonModel(OlaSkeleton* skel,OlaResourceMng* res_mng):
mChannelset(0),
mResourceMng(res_mng),
mSkeleton(skel),
mCPUSkin(0)
{
	mChannelset = new OlaActionChannelSet();
	
	mChannelset->initialize(mSkeleton);
}

OlaSkeletonModel::~OlaSkeletonModel()
{
	if(mCPUSkin)
	{
		delete mCPUSkin;
		mCPUSkin = 0;
	}
	delete mChannelset;
}

void OlaSkeletonModel::update( float elasped )
{
	mChannelset->update(elasped);

	if(mCPUSkin)
	{
		mCPUSkin->updateCPUSkin(mChannelset);
	}
}

void OlaSkeletonModel::setCpuSkin( OlaMesh* skin_res )
{
	if(mCPUSkin)
		delete mCPUSkin;

	mCPUSkin = new OlaSkin(skin_res);

	mCPUSkin->attachSkeleton(mSkeleton);
}

bool OlaSkeletonModel::setAction( const char* act_name )
{
	mChannelset->channel(0)->setAction(act_name,true);
	mChannelset->setPassedTime(0.0f);
	return true;
}

void OlaSkeletonModel::_updateCPUSkin()
{

}

void OlaSkeletonModel::setPassedTime( float t )
{
	mChannelset->setPassedTime(t);

	if(mCPUSkin)
	{
		mCPUSkin->updateCPUSkin(mChannelset);
	}
}

OlaAction* OlaSkeletonModel::currentAction(int channel)
{
	return mChannelset->channel(0)->currentAction();
}

OlaBoneState* OlaSkeletonModel::boneState( int index )
{
	return mChannelset->boneState(index);
}