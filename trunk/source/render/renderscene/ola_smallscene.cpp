#include "ola_smallscene.h"
#include "../ola_light.h"

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
	unsigned int f = obj->flags();

	if (f & TRANSFORM_OBJPARAM::OBJFLAG_LIGHT)
	{
		OlaLight* l = dynamic_cast<OlaLight*>(obj);
		assert( l && "OlaLight* l = dynamic_cast<OlaLight*>(obj)");
		OlaRenderScene* ls = l->scene();
		if (ls && ls != this)
		{
			ls->detachObj(l);
		}
		l->setScene(this);
		mLights.add(l);
	}		
}

void OlaSmallScene::detachObj( OlaTransformObj* obj )
{
	unsigned int f = obj->flags();

	if (f & TRANSFORM_OBJPARAM::OBJFLAG_LIGHT)
	{
		OlaLight* l = dynamic_cast<OlaLight*>(obj);
		assert( l && "OlaLight* l = dynamic_cast<OlaLight*>(obj)");
		if (mLights.contains(l))
		{
			l->setScene(0);
			mLights.remove(l);
		}
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