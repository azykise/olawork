#include "ola_smallscene.h"
#include "../ola_light.h"

OlaSmallScene::OlaSmallScene( const char* name ):
OlaSceneBase(name)
{

}

OlaSmallScene::~OlaSmallScene()
{

}

void OlaSmallScene::attachCell( OlaSceneCellImpl* cell )
{
	if(!mRoot->cells().contains(cell))
	{
		mRoot->cells().add(cell);
	}				
}

void OlaSmallScene::detachCell( OlaSceneCellImpl* cell )
{
	if(mRoot->cells().contains(cell))
	{
		mRoot->cells().remove(cell);
	}
}

void OlaSmallScene::initialize()
{
	OlaSceneBase::initialize();
	mRoot = new OlaSceneNode("Root",0);
}

void OlaSmallScene::release()
{
	OlaSceneBase::release();
}

CellList& OlaSmallScene::visibleCells()
{
	return mRoot->cells();
}