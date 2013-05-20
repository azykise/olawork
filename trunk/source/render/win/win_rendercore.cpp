#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "win_renderwindow.h"

#include "../ola_resourcemng.h"
#include "../ola_primitive.h"

#include "../ola_mesh.h"
#include "../ola_skin.h"

#include "../ola_kernelobj.h"

#include "../parser/ola_dml.h"

#include "win_impls.h"
#include "win_rendercore.h"

#include "../ola_device.h"

#include "../ola_model.h"

#include "../impls/ola_geometry_impl.h"
#include "../impls/ola_light_impl.h"

#define  lg(...) printf(__VA_ARGS__)

OlaRenderCore::OlaRenderCore():
mRender(0),
mSceneMng(0),
mKernelObjs(0),
mCurrentWindow(0)
{
	
}

OlaRenderCore::~OlaRenderCore()
{
	release();
}

OlaRenderDevice* gDevice = 0;
OlaRenderDevice* GetRenderDevice()
{
	return gDevice;
}

bool OlaRenderCore::initialize(unsigned int hwnd)
{
	RECT rect;
	::GetWindowRect((HWND)hwnd,&rect);
	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;

	mDevice = SpawnGLRenderDevice();
	mDevice->init(hwnd);

	gDevice = mDevice;

	mKernelObjs = CreateSmallKernelObjTrunk();

	mRender = new OlaRender();
	mRender->onInitRender(w,h,mDevice);

	mSceneMng = new OlaRenderSceneMng(mRender);
	mSceneMng->initialize();

	mRender->setRenderScene(mSceneMng->scene());

	olaVec3 cam_pos(0,-5,0);	
	olaVec3 cam_tag(0,0,0);

	cam_pos.Set(-169.91f,322.12f,140.78f);	
	cam_tag.Set(-70.37f,217.91f,83.68f);

	cam_pos.Set(122.31f,82.37f,9.61f);		
	cam_tag.Set(73.37f,-61.42f,-21.68f);

	tViewParam vp;
	mRender->getViewFrustum(0)->toViewParam(&vp);

	vp.pos[0] = cam_pos.x;	vp.pos[1] = cam_pos.y;	vp.pos[2] = cam_pos.z;

	vp.lookat[0] = cam_tag.x;
	vp.lookat[1] = cam_tag.y;
	vp.lookat[2] = cam_tag.z;

	mRender->getViewFrustum(0)->fromViewParam(&vp);

	return true;
}

void OlaRenderCore::release()
{
	if(mKernelObjs)
	{
		mKernelObjs->removeUnusedObj();
		delete mKernelObjs;
		mKernelObjs = 0;
	}

	if (mRender)
	{
		mRender->onRelease();
		delete mRender;
		mRender = 0;
	}

	if(mSceneMng)
	{
		delete mSceneMng;
		mSceneMng = 0;
	}

	RenderWindowTable::iterator wi = mWindows.begin();
	while(wi != mWindows.end())
	{
		wi->second->release();
		delete wi->second;
		wi++;
	}
	mWindows.clear();

	if(mDevice)
	{
		delete mDevice;
		mDevice = 0;
	}
}

void OlaRenderCore::beginDraw()
{
	if(mCurrentWindow)
	{
		int w = mCurrentWindow->getWidth();
		int h = mCurrentWindow->getHeight();

		if( w != mRender->getScreenW() || h != mRender->getScreenH() )
		{
			mRender->onResize(w,h);
		}
	}
}

void OlaRenderCore::drawSceneActors()
{
	if(mRender)
	{
		OlaRenderScene* current_scene = mSceneMng->scene();

		current_scene->updateScene(mRender->getViewFrustum(0));

		OlaArray<OlaTransformObj*>& actived_tobjs = current_scene->activedObjs();
		unsigned int tobj_num = actived_tobjs.size();

		for (unsigned int i = 0 ; i < tobj_num ; i++)
		{
			OlaTransformObj* tobj = actived_tobjs[i];
			unsigned int kernel_num = tobj->kernelObjCount();

			for (unsigned int n = 0 ; n < kernel_num ; n++)
			{
				OlaKernelObj* kobj = tobj->kernelObj(n);
				switch (kobj->enabled())
				{
				case OlaKernelObj::ES_ENABLE_ALL:
					kobj->updateInternal(0.0f,tobj);
					kobj->renderInternal(mRender);
					break;
				case OlaKernelObj::ES_ENABLE_UPDATE:
					kobj->updateInternal(0.0f,tobj);
					break;
				case OlaKernelObj::ES_ENABLE_RENDER:
					kobj->renderInternal(mRender);
					break;								
				}
			}
		}

		mRender->onRender(0.0f);
	}
}

void OlaRenderCore::endDraw()
{
	if(mCurrentWindow)
	{
		mCurrentWindow->swapBuffer();
	}
}

void OlaRenderCore::getView(ola::tViewParam* vp)
{
	OlaVFrustum* vf = mRender->getViewFrustum(0);
	tViewParam t;
	vf->toViewParam(&t);
	memcpy(vp,&t,sizeof(tViewParam));
}

void OlaRenderCore::setView(ola::tViewParam* vp)
{
	OlaVFrustum* vf = mRender->getViewFrustum(0);
	tViewParam t;
	memcpy(&t,vp,sizeof(tViewParam));
	vf->fromViewParam(&t);	
}

void OlaRenderCore::setCamera(int index,ola::vec3& pos,ola::vec3& tag)
{
	olaVec3 _pos(pos.x,pos.y,pos.z);
	olaVec3 _tag(tag.x,tag.y,tag.z);

	mRender->setCamera(index,_pos,_tag);	
}

void OlaRenderCore::getCamera(int index,ola::vec3& pos,ola::vec3& tag)
{
	olaVec3 _pos,_tag;

	mRender->getCamera(index,_pos,_tag);

	memcpy(&pos,&_pos,sizeof(olaVec3));
	memcpy(&tag,&_tag,sizeof(olaVec3));
}

void OlaRenderCore::getViewMatrix(float* mat)
{
	olaMat4 vmat = mRender->getViewFrustum(0)->getViewMatrix();
	memcpy(mat,vmat.ToFloatPtr(),sizeof(olaMat4));
}

void OlaRenderCore::getProjMatrix(float* mat)
{
	olaMat4 vmat = mRender->getViewFrustum(0)->getProjMatrix();
	memcpy(mat,vmat.ToFloatPtr(),sizeof(olaMat4));
}

ola::IRenderWindow*	OlaRenderCore::createWindow(ola::RenderWindowInfo* info)
{
	int index = mWindows.size();
	OlaRenderWindow* w = new OlaRenderWindow(index,this);
	w->resizeRenderWindow(info);
	mWindows[index] = w;
	return w;
}

void OlaRenderCore::setWindow(ola::IRenderWindow* wnd)
{
	int index = wnd->getIndex();

	if(mCurrentWindow && index == mCurrentWindow->getIndex())
		return;

	RenderWindowTable::iterator wi = mWindows.find(index);
	if(wi != mWindows.end())
	{
		mCurrentWindow = wi->second;
		mCurrentWindow->setActive();
	}	
}

ola::IPrimitive* OlaRenderCore::createPrimitive(ola::PrimitiveType type)
{
	OlaPrimitive* prim = new OlaPrimitive(OlaPrimitiveParam::PRI_LINE);
	OlaMaterial* pri_mat = mRender->getResourceMng()->getPrimitiveMaterial();
	prim->renderop()->material(pri_mat);

	OlaPrimitiveImpl* impl = new OlaPrimitiveImpl(prim,mSceneMng);
	//mSceneMng->prims().push_back(prim);
	return impl;
}

ola::IStaticModel* OlaRenderCore::createStaticModel(const char* filename)
{
	OlaResourceMng* res_mng = mRender->getResourceMng();

	OlaAsset* xml_asset = res_mng->getLoader()->load(filename,false);

	olastring dml_filename(filename);

	tDmlFileInfo dmlInfo;
	dmlInfo.DMLAssetpath = OlaUtility::FilePathToAssetPath(dml_filename);

	OlaMeshRenderer* dml = 0;

	OlaDMLParser parser(res_mng->pools());
	parser.parseDMLInfoFromData(xml_asset->data,xml_asset->length,&dmlInfo);
	delete xml_asset;

	OlaKernelObj* dml_kobj = mKernelObjs->kernelObj(dmlInfo.DMLAssetpath.c_str());
	if (dml_kobj == 0)
	{
		dml = new OlaMeshRenderer(dmlInfo.DMLAssetpath.c_str());
		parser.fillDML(&dmlInfo,dml);
		mKernelObjs->enTrunk(dml);
	}
	else
	{
		dml = static_cast<OlaMeshRenderer*>(dml_kobj);
	}		

	OlaStaticModelImpl* impl = new OlaStaticModelImpl(dml,mRender);
	return impl;
}

ola::IActor* OlaRenderCore::createActor( const char* chr_filename )
{	
	return 0;
}

void OlaRenderCore::pushRender(ola::IPrimitive* r)
{
	OlaPrimitiveImpl* impl = static_cast<OlaPrimitiveImpl*>(r);
	mRender->pushToRender(impl->mPrimitive);
}

ola::ICharacter* OlaRenderCore::createCharacter( const char* chr_filename )
{
	OlaSkeletonModel* skel_model = mSceneMng->loadSkeletonModelFromCHR(chr_filename);

	OlaCharacterImpl* impl = new OlaCharacterImpl(skel_model);

	return impl;
}

ola::ILight* OlaRenderCore::createLight( const char* name )
{	
	OlaLight* l = new OlaLight();
	mKernelObjs->enTrunk(l);

	OlaLightImpl* impl = new OlaLightImpl(l);
	return impl;
}

void OlaRenderCore::clearResourceCache( const char* res_name )
{
	mRender->getResourceMng()->clearResourceCache(res_name);
}

int OlaRenderCore::getResourceNum( const char* res_name )
{
	return mRender->getResourceMng()->getResourceNum(res_name);
}

ola::IScene* OlaRenderCore::createScene( const char* name ,ola::IScene::SCENE_TYPE t)
{
	OlaRenderScene* scene = 0;
	switch(t)
	{
	case ola::IScene::SCENE_SMALL:
		scene = mSceneMng->createScene(name,OlaRenderSceneMng::SCENE_SMALL);
		break;
	case ola::IScene::SCENE_OUTDOOR:
		break;
	case ola::IScene::SCENE_INDOOR:
		break;
	}

	if(scene)
	{
		OlaSceneImpl* impl = new OlaSceneImpl(scene,mSceneMng);
		return impl;
	}
	else
		return 0;
}

void OlaRenderCore::setRenderingScene( ola::IScene* scene )
{
	OlaSceneImpl* impl = dynamic_cast<OlaSceneImpl*>(scene);
	mSceneMng->setRenderScene(impl->mScene);
	mRender->setRenderScene(mSceneMng->scene());
}
