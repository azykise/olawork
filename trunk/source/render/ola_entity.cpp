#include "ola_entity.h"
#include "ola_scene.h"

ISpatialObj::ISpatialObj():
mScene(0)
{

}

ISpatialObj::~ISpatialObj()
{
	if (mScene)
	{
		mScene->detachObj(this);
		mScene = 0;
	}
}