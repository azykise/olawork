#ifndef _OLA_INCLUDE_CELL_IMLP_H__
#define _OLA_INCLUDE_CELL_IMLP_H__

#include "../../interface/ola_iscene.h"
using namespace ola;

class OlaSceneImpl;
class OlaSceneCellImpl : public ola::ISceneCell
{
public:
	OlaSceneCellImpl();
	virtual ~OlaSceneCellImpl();

	virtual void combineShape( tAABB* aabb );

	virtual void combineShape( tVec3* vec3s,int type );

	virtual void combineShape( float radius );

	virtual IScene* scene();

	virtual void detach();

	virtual ITransform* transform();

	virtual void aabb( tAABB* out_aabb );

	virtual void obb( tVec3* out_obb );

	virtual float radius();

	virtual void setGroupID( int group_id );

	virtual int groupID();

	virtual bool visible();

	OlaSceneImpl* mScene;
	bool mVisible;
	int mGroupID;
};

#endif