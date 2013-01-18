#include "ola_scene.h"
#include "ola_light.h"

#include "ola_scenemng.h"

OlaSceneBase* OlaRenderSceneMng::scene( const char* name )
{
	return mScene0;
}

OlaSceneBase::OlaSceneBase( const char* name):
mName(name),
mRoot(0)
{
	
}

OlaSceneBase::~OlaSceneBase()
{
	
}

void OlaSceneBase::initialize()
{
	release();	
}

void OlaSceneBase::release()
{
	if (mRoot)
	{
		delete mRoot;
		mRoot = 0;
	}
}

OlaSceneNode* OlaSceneBase::findNode( const char* name )
{	
	if (!strcmp(name,mRoot->name().c_str()))	
		return mRoot;	
	else
		return mRoot->findSubNode(name);
}
