#ifndef _OLA_INCLUDE_SCENE_IMLP_H__
#define _OLA_INCLUDE_SCENE_IMLP_H__

#include "../../interface/ola_iscene.h"
using namespace ola;

class OlaSceneBase;

class OlaSceneImpl : public ola::IScene
{
public:
	OlaSceneImpl(const char* name,IScene::SCENE_TYPE t);

	virtual ~OlaSceneImpl();

	virtual const char* name();

	virtual ISceneCell* createCell();

	virtual void attachCell( ISceneCell* cell );

	virtual void setCamera( ICamera* cam );

	virtual void update();

	virtual ISceneCell** visibleCells( int group_id,int* num );

	OlaSceneBase* mScene;
};

#endif