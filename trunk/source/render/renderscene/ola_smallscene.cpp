#include "ola_smallscene.h"
#include "../ola_light.h"

#include "../ola_camera.h"

OlaRenderScene* newSmallScene(const char* name,OlaRenderSceneMng* mng)
{
	return new OlaSmallScene(name,mng);
}

OlaSmallScene::OlaSmallScene( const char* name,OlaRenderSceneMng* mng ):
OlaRenderScene(name,mng)
{

}

OlaSmallScene::~OlaSmallScene()
{

}

void OlaSmallScene::attachObj( OlaTransformObj* obj )
{
	if (!mTransformObjs.contains(obj))
	{
		mTransformObjs.push_back(obj);
	}
}

void OlaSmallScene::detachObj( OlaTransformObj* obj )
{
	if (mTransformObjs.contains(obj))
	{
		mTransformObjs.remove(obj);
	}
}

void OlaSmallScene::initialize()
{
	OlaRenderScene::initialize();
	mRoot = new OlaSceneNode("Root",0);
}

void OlaSmallScene::release()
{
	OlaRenderScene::release();
	mLights.clear();
}

const OlaRenderScene::LightList* OlaSmallScene::lights( bool all /*= false */ )
{
	return &mLights;
}

OlaArray<OlaTransformObj*>& OlaSmallScene::activedObjs()
{
	return mTransformObjs;
}

void OlaSmallScene::updateScene( OlaVFrustum* view )
{
	for (unsigned int i = 0 ; i < mTransformObjs.size() ; i++)
	{
		mTransformObjs[i]->visiable(TRANSFORM_OBJPARAM::OBJVIS_ALL);
	}
}
