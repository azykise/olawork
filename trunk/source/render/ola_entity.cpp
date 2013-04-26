#include "ola_entity.h"
#include "ola_scene.h"

OlaTransformObj::OlaTransformObj():
mScene(0)
{

}

OlaTransformObj::~OlaTransformObj()
{
	if (mScene)
	{
		mScene->detachObj(this);
		mScene = 0;
	}
}