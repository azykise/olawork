#ifndef _OLA_INCLUDE_SMALLRENDERSCENE_H__
#define _OLA_INCLUDE_SMALLRENDERSCENE_H__

#include "../ola_scenemng.h"
#include "../ola_scene.h"


class OlaSmallScene : public OlaSceneBase
{
public:
	OlaSmallScene(const char* name);
	virtual ~OlaSmallScene();

	virtual void initialize();
	virtual void release();

	virtual void attachCell(OlaSceneCellImpl* obj);
	virtual void detachCell(OlaSceneCellImpl* obj);

	virtual CellList& visibleCells();
protected:
	
};


#endif