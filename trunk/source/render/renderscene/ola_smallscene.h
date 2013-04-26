#ifndef _OLA_INCLUDE_SMALLRENDERSCENE_H__
#define _OLA_INCLUDE_SMALLRENDERSCENE_H__

#include "../ola_scenemng.h"
#include "../ola_scene.h"


class OlaSmallScene : public OlaRenderScene
{
public:
	OlaSmallScene(const char* name,OlaRenderSceneMng* mng);
	virtual ~OlaSmallScene();

	virtual void initialize();
	virtual void release();

	virtual void attachObj(OlaTransformObj* obj);
	virtual void detachObj(OlaTransformObj* obj);

	virtual const LightList* lights( bool all = false );

protected:

	LightList mLights;
};


#endif