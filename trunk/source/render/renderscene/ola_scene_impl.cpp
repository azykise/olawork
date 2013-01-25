#include <exception>
#include "ola_scene_impl.h"
#include "ola_cell_impl.h"

#include "ola_smallscene.h"

OlaSceneImpl::OlaSceneImpl( const char* name ,IScene::SCENE_TYPE t):
mScene(0)
{
	switch(t)
	{
	case IScene::SCENE_SMALL:
		mScene = new OlaSmallScene(name);
		break;
	}
}

OlaSceneImpl::~OlaSceneImpl()
{
	if (mScene)
	{
		delete mScene;
		mScene = 0;
	}
}

ISceneCell* OlaSceneImpl::createCell()
{
	return new OlaSceneCellImpl();
}

void OlaSceneImpl::attachCell( ISceneCell* cell )
{
	OlaSceneCellImpl* cell_impl = static_cast<OlaSceneCellImpl*>(cell);
	mScene->attachCell(cell_impl);
}

void OlaSceneImpl::setCamera( IViewFrustum* cam )
{
	throw std::exception("The method or operation is not implemented.");
}

void OlaSceneImpl::update()
{
	throw std::exception("The method or operation is not implemented.");
}

ISceneCell** OlaSceneImpl::visibleCells( int group_id,int* num )
{
	CellList& vis_cell = mScene->visibleCells();
	if (vis_cell.size())
	{
		*num = vis_cell.size();
		return (ISceneCell**)(&(vis_cell[0]));
	}
	else
	{
		*num = 0;
		return 0;
	}

}

const char* OlaSceneImpl::name()
{
	return mScene->name().c_str();
}
