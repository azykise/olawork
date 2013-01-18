#include "../ola_render.h"
#include "../ola_light.h"
#include "../model.h"

#include "../ola_renderpipeline.h"

#include "../ola_material.h"
#include "../ola_mesh.h"
#include "../ola_resourcemng.h"

#include "../ola_renderqueue.h"
#include "../ola_postprocess.h"

#include "../ola_scenemng.h"
#include "../ola_device.h"

#define lgGLError(x) 	if(glGetError() != GL_NO_ERROR){lg(x);}

#include "ola_perlightpipeline.h"

const char SCENEINPUT_RENDERTARGET[] = "sceneinput_rt";
const char SHADOWMAP_RENDERTARGET[] = "shadowmap_rt";
const char SCENEFINAL_QUAD[]	=	"scenefinal_quad";
const int SHADOW_MAP_SCALE = 2;


OlaPerLightPipeline::OlaPerLightPipeline(OlaRender *render):
OlaForwardPipeline(render),
mChain(0)
{

}

OlaPerLightPipeline::~OlaPerLightPipeline()
{

}

void OlaPerLightPipeline::initialize()
{
    OlaForwardPipeline::initialize();
    
	mChain = new OlaScreenQuadChain();
	mChain->initialize();

    int w = mRender->getScreenW();
    int h = mRender->getScreenH();

	mRenderQueue->initGroup(OlaRenderQueue::SHADOWCAST_GROUP,mRender->getResourceMng());

	mGLOrgRT = GetRenderDevice()->spawnRenderTarget("gl_default_rt");
	mGLOrgRT->initialize(w,h);

    mSceneInputRT = GetRenderDevice()->spawnRenderTarget(SCENEINPUT_RENDERTARGET); 
	mSceneInputRT->initialize(w,h);

	mShadowmapRT = GetRenderDevice()->spawnRenderTarget(SHADOWMAP_RENDERTARGET);
	mShadowmapRT->initialize(w * SHADOW_MAP_SCALE,h * SHADOW_MAP_SCALE);
    
 //   OlaMaterial* scenefinal_mat = mRender->getMaterial("data/pp_test.mat","pp_test");
	//scenefinal_mat->addRef();

	//mSceneFinalQuad = mChain->creatScreenQuad(SCENEFINAL_QUAD,scenefinal_mat);
	//mFinalQuadRenderOp = new OlaRenderOp(mSceneFinalQuad);
 //   mSceneFinalQuad->setPixelW(w);
	//mSceneFinalQuad->setPixelH(h);
}

void OlaPerLightPipeline::release()
{
    OlaForwardPipeline::release();
	if(mChain)
	{
		delete mChain;
		mChain = 0;
	}

	if(mSceneInputRT)
	{
		delete mSceneInputRT;
		mSceneInputRT = 0;
	}

	if(mShadowmapRT)
	{
		delete mShadowmapRT;
		mShadowmapRT = 0;
	}

	if(mGLOrgRT)
	{
		delete mGLOrgRT;
		mGLOrgRT = 0;
	}
}

void OlaPerLightPipeline::resize(int w,int h)
{
	mGLOrgRT->resize(w,h);
	mSceneInputRT->resize(w,h);
	mSceneFinalQuad->setPixelW(w);
	mSceneFinalQuad->setPixelH(h);
	mShadowmapRT->resize(w,h);
}

void OlaPerLightPipeline::_renderScreenFinalQuad()
{
	GetRenderDevice()->setViewport(0,0,mSceneFinalQuad->pixelW(),mSceneFinalQuad->pixelH());
    OlaMaterial* material = mSceneFinalQuad->orgMaterial();

	OlaMaterialParam::PARAM_VALUE* diffuse_value = material->symbol(OlaMaterialParam::DIFFUSE0);
	if(diffuse_value)
	{
		diffuse_value->data = mSceneInputRT;//;mShadowmapRT mSceneInputRT
	}

	GetRenderDevice()->bindRenderState(material->shader(),&mGlobalShaderConst);
	GetRenderDevice()->flushMaterialInfo(material);
    
    mFinalQuadRenderOp->worldtrans.Identity();
    mViewMatrix.Identity();
    mProjMatrix.Identity();

	GetRenderDevice()->drawCall(mFinalQuadRenderOp,&mGlobalShaderConst);

    GetRenderDevice()->wipeRenderState(material->shader());
}

void OlaPerLightPipeline::execute()
{  
//	setRenderTarget(mShadowmapRT);

//	_renderShadowMap(); 

	setRenderTarget(mSceneInputRT);   
    
	_renderGroupedModels();	

	setRenderTarget(mGLOrgRT);		
    
	_renderScreenFinalQuad();   

	mRender->setTotalTris(mRenderQueue->totalTris());
}

void OlaPerLightPipeline::_renderShadowMap()
{
	//const OlaSceneBase::LightList& dir_lights = *(mCurrentScene->lights());;

	//olaVec3 pos = *(dir_lights[0]->position());
	//olaVec3 lat = *(dir_lights[0]->lookatPT());

	//mLightVFrustum->setEyePt(pos);
	//mLightVFrustum->setLookPt(lat);

	pushMatrix(OlaRenderPipeLine::PROJ_MATRIX,mLightVFrustum->getProjMatrix());
	pushMatrix(OlaRenderPipeLine::VIEW_MATRIX,mLightVFrustum->getViewMatrix());

	GetRenderDevice()->clearViewport(0,0,0,0);
	OlaRenderGroup* shadowcast_group = mRenderQueue->group(OlaRenderQueue::SHADOWCAST_GROUP);
	if(!shadowcast_group)
		return;

	OlaRenderGroup::RenderOpQueueTable& shadow_queues = shadowcast_group->opqueues();
	OlaRenderGroup::RenderOpQueueTable::iterator shadow_i = shadow_queues.begin();
	while (shadow_i != shadow_queues.end())
	{
		OlaShader* shader = shadow_i->first;
		OlaRenderGroup::RenderOpQueue* queue = shadow_i->second;

		GetRenderDevice()->bindRenderState(shader,&mGlobalShaderConst);

		for (int i = 0 ; i < queue->size() ; i++)
		{
			OlaRenderOp* op = queue->at(i);

			GetRenderDevice()->drawCall(op,&mGlobalShaderConst);
		}
		
		GetRenderDevice()->wipeRenderState(shader);

		shadow_i++;
	}
	mRenderQueue->clear(OlaRenderQueue::SHADOWCAST_GROUP);

	popMatrix(OlaRenderPipeLine::PROJ_MATRIX);
	popMatrix(OlaRenderPipeLine::VIEW_MATRIX);
}
