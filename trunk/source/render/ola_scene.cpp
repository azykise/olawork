#include "ola_scene.h"
#include "ola_light.h"

#include "ola_scenemng.h"

OlaRenderScene* OlaRenderSceneMng::scene( const char* name )
{
	return mScene0;
}

OlaRenderScene::OlaRenderScene( const char* name, OlaRenderSceneMng* mng ):
mName(name),
mParent(mng),
mRoot(0),
mTrunk(0)
{
	
}

OlaRenderScene::~OlaRenderScene()
{
	
}

void OlaRenderScene::initialize()
{
	release();	
}

void OlaRenderScene::release()
{
	if (mRoot)
	{
		delete mRoot;
		mRoot = 0;
	}
}

const OlaRenderScene::LightList* OlaRenderScene::lights( bool all /*= false*/ )
{
	return 0;
}

OlaSceneNode* OlaRenderScene::findNode( const char* name )
{	
	if (!strcmp(name,mRoot->name().c_str()))	
		return mRoot;	
	else
		return mRoot->findSubNode(name);
}

OlaSceneTrunk* OlaRenderScene::trunk()
{
	return mTrunk;
}
