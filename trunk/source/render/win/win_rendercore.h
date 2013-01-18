#pragma once

#include "../../interface/ola_engine.h"

#include "../ola_render.h"
#include "../ola_scenemng.h"


class OlaRenderWindow;
class OlaRenderDevice;
class OlaRenderCore : public ola::IRenderCore
{
	typedef std::map<int,OlaRenderWindow*> RenderWindowTable;
public:
	OlaRenderCore();

	virtual ~OlaRenderCore();

	virtual bool initialize(unsigned int hwnd);

	virtual void release();

	virtual void beginDraw();
	virtual void drawSceneActors();
	virtual void endDraw();

	virtual void getView(ola::tViewParam* vp);
	virtual void setView(ola::tViewParam* vp);
	virtual void setCamera(int index,ola::tVec3& pos,ola::tVec3& tag);
	virtual void getCamera(int index,ola::tVec3& pos,ola::tVec3& tag);
	virtual void getViewMatrix(float* mat);
	virtual void getProjMatrix(float* mat);

	virtual ola::IRenderWindow*	createWindow(ola::RenderWindowInfo* info);
	virtual void setWindow(ola::IRenderWindow* wnd);

	virtual void pushRender(ola::IPrimitive* r);
	virtual void pushRender(ola::IStaticModel* r);
	virtual void pushRender(ola::IRenderable* r);

	virtual void pushHardwareSelect(ola::IPrimitive* primitive,unsigned int sel_id){};

	virtual void beginHardwareSelect(int x,int y,int w,int h) {};
	virtual unsigned int* endHardwareSelect(int& num) { num = 0 ; return 0;};

	virtual ola::IPrimitive* createPrimitive(ola::PrimitiveType type);
	virtual ola::IStaticModel* createStaticModel(const char* filename);
	virtual ola::ICharacter* createCharacter(const char* chr_filename);
	virtual ola::IActor* createActor(const char* chr_filename);
	virtual ola::ILight* createLight(const char* name);
	virtual void releaseObject(ola::IObject* object){delete object;}

	virtual void clearResourceCache(const char* res_name);
	virtual int getResourceNum(const char* res_name);

	virtual int loadSkeletonResourceFromASE(const char* name,const char* filename);
	virtual int removeSkeletonResource(const char* name);

	virtual int loadActionResourceFromASE(const char* act_name,const char* ase_filename);
	virtual int removeActionResource(const char* act_name);
	virtual int loadBodyPartResourceFromBPT(const char* bpt_name,const char* bpt_filename);
	virtual int removeBodyPartResource(const char* bpt_name);

	virtual ola::IScene* createScene( const char* name ,ola::IScene::SCENE_TYPE t = ola::IScene::SCENE_SMALL);
	virtual void setRenderingScene(ola::IScene* scene);

	inline OlaRenderDevice* device() {return mDevice;}

protected:
	OlaRender* mRender;
	OlaRenderSceneMng* mSceneMng;

	OlaRenderWindow* mCurrentWindow;
	RenderWindowTable mWindows;

	OlaRenderDevice* mDevice;
};