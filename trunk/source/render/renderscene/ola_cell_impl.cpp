#include <exception>

#include "../ola_scene.h"
#include "ola_scene_impl.h"
#include "ola_cell_impl.h"

OlaSceneCellImpl::OlaSceneCellImpl():
mScene(0),
mGroupID(0),
mVisible(false)
{

}

OlaSceneCellImpl::~OlaSceneCellImpl()
{

}
void OlaSceneCellImpl::combineShape( tAABB* aabb )
{
	throw std::exception("The method or operation is not implemented.");
}

void OlaSceneCellImpl::combineShape( tVec3* vec3s,int type )
{
	throw std::exception("The method or operation is not implemented.");
}

void OlaSceneCellImpl::combineShape( float radius )
{
	throw std::exception("The method or operation is not implemented.");
}

IScene* OlaSceneCellImpl::scene()
{
	return mScene;
}

void OlaSceneCellImpl::detach()
{
	mScene->mScene->detachCell(this);
	mScene = 0;
}

ITransform* OlaSceneCellImpl::transform()
{
	throw std::exception("The method or operation is not implemented.");
}

void OlaSceneCellImpl::aabb( tAABB* out_aabb )
{
	throw std::exception("The method or operation is not implemented.");
}

void OlaSceneCellImpl::obb( tVec3* out_obb )
{
	throw std::exception("The method or operation is not implemented.");
}

float OlaSceneCellImpl::radius()
{
	throw std::exception("The method or operation is not implemented.");
}

void OlaSceneCellImpl::setGroupID( int group_id )
{
	mGroupID = group_id;
}

int OlaSceneCellImpl::groupID()
{
	return mGroupID;
}

bool OlaSceneCellImpl::visible()
{
	return mVisible;
}

