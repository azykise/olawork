#include <iostream>

#include "../ola_render.h"
#include "../ola_light.h"
#include "../model.h"

#include "../ola_renderpipeline.h"

#include "../ola_material.h"
#include "../ola_mesh.h"
#include "../ola_resourcemng.h"
#include "../ola_primitive.h"

#include "../ola_renderqueue.h"
#include "../ola_postprocess.h"

#include "../ola_scenemng.h"
#include "../ola_device.h"

#define lgGLError(x) 	if(glGetError() != GL_NO_ERROR){lg(x);}

#include "ola_forwardpipeline.h"


OlaForwardPipeline::OlaForwardPipeline(OlaRender* render):
OlaRenderPipeLine(render),
mShadowmapRT(0),
mLightVFrustum(0)
{
	mShadowBiasMatrix = olaMat4(0.5f, 0.0f, 0.0f, 0.0f,
								0.0f, 0.5f, 0.0f, 0.0f,
								0.0f, 0.0f, 0.5f, 0.0f,
								0.5f, 0.5f, 0.5f, 1.0f);
	mShadowmapTexlProjMatrix.Identity();
}

OlaForwardPipeline::~OlaForwardPipeline()
{
	release();
}

void OlaForwardPipeline::initialize()
{
	mRenderQueue = new OlaRenderQueue();

	mRenderQueue->initGroup(OlaRenderQueue::SOILD_GROUP,mRender->getResourceMng());
	mRenderQueue->initGroup(OlaRenderQueue::PRIM_GROUP,mRender->getResourceMng());

    float w = mRender->getScreenW();
    float h = mRender->getScreenH();

	mLightVFrustum = new OlaVFrustum();
	tViewParam vp;
	memset(&vp,0,sizeof(tViewParam));
	vp.aspect = 1.0f;
	vp.far_dist = 1001.f;
	vp.near_dist = 1.0f;
	vp.fovy = olaMath::M_DEG2RAD * 90.0f;
	vp.lookat[0] = 0; vp.lookat[1] = 0; vp.lookat[2] = 0;
	vp.pos[0] = 0; vp.pos[1] = 0; vp.pos[2] = 5;
	mLightVFrustum->fromViewParam(&vp);
}

void OlaForwardPipeline::release()
{
	if(mRenderQueue)
	{
		delete mRenderQueue;
		mRenderQueue = 0;
	}

	if(mLightVFrustum)
	{
		delete mLightVFrustum;
		mLightVFrustum = 0;
	}
}

void OlaForwardPipeline::execute()
{
	GetRenderDevice()->setViewport(0,0,mRender->getScreenW(),mRender->getScreenH());
	GetRenderDevice()->clearViewport(0.5f,0.5f,0.5f,1.0f);

	if (mViewMatrixs.size())
	{
		mGlobalShaderConst.ViewMat = *(mViewMatrixs.top());
	}
	else
		mGlobalShaderConst.ViewMat = mViewMatrix;

	if(mProjMatrixs.size())
	{
		mGlobalShaderConst.ProjMat = *(mProjMatrixs.top());
	}
	else
		mGlobalShaderConst.ProjMat = mProjMatrix;	

	mGlobalShaderConst.EyePos.Set(mGlobalShaderConst.ViewMat[0][3],mGlobalShaderConst.ViewMat[1][3],mGlobalShaderConst.ViewMat[2][3]);

	const OlaRenderScene::LightList& dir_lights = *(mCurrentScene->lights());

	mGlobalShaderConst.LightNum = dir_lights.size();
	for(int i = 0 ; i < dir_lights.size() ; i++)
	{
		mGlobalShaderConst.LightParam[i].pos.Set(dir_lights[i]->position()->x,dir_lights[i]->position()->y,dir_lights[i]->position()->z);
		mGlobalShaderConst.LightParam[i].dst.Set(dir_lights[i]->lookatPT()->x,dir_lights[i]->lookatPT()->y,dir_lights[i]->lookatPT()->z);
	}

	_renderGroupedModels();

    mRender->setTotalTris(mRenderQueue->totalTris());    	
}

void OlaForwardPipeline::_renderWireframeGroup()
{
	OlaPrimitiveGroup* prim_group = mRenderQueue->primGroup();
	if(!prim_group)
		return;

	OlaShader* shader = prim_group->mat()->shader();

	GetRenderDevice()->bindRenderState(shader,&mGlobalShaderConst);
	GetRenderDevice()->flushMaterialInfo(prim_group->mat());

	OlaPrimitiveGroup::PrimitiveList& prim_list = prim_group->prims();
	OlaPrimitiveGroup::PrimitiveList::iterator p_i = prim_list.begin();
	while (p_i != prim_list.end())
	{
		OlaPrimitive* pri = *p_i;

		GetRenderDevice()->drawCall(pri->renderop(),&mGlobalShaderConst);

		p_i++;
	}

	GetRenderDevice()->wipeRenderState(shader);

	prim_list.clear();
}

void OlaForwardPipeline::_renderSingleModelGroup(OlaShader* shader,OlaRenderGroup::RenderOpQueue* queue)
{
	GetRenderDevice()->bindRenderState(shader,&mGlobalShaderConst);
	for (int i = 0 ; i < queue->size() ; i++)
	{
		OlaRenderOp* op = queue->at(i);

		GetRenderDevice()->flushMaterialInfo(op->material());
		GetRenderDevice()->drawCall(op,&mGlobalShaderConst);
	}

	GetRenderDevice()->wipeRenderState(shader);
}

void OlaForwardPipeline::_renderSoildGroup()
{
	OlaRenderGroup* soild_group = mRenderQueue->group(OlaRenderQueue::SOILD_GROUP);
	if(!soild_group)
		return;

	OlaRenderGroup::RenderOpQueueTable& soild_queues = soild_group->opqueues();
	OlaRenderGroup::RenderOpQueueTable::iterator soild_i = soild_queues.begin();
	while (soild_i != soild_queues.end())
	{
		_renderSingleModelGroup(soild_i->first,soild_i->second);
		soild_i++;
	}
	mRenderQueue->clear(OlaRenderQueue::SOILD_GROUP);
}

void OlaForwardPipeline::_renderGroupedModels()
{	
	_renderSoildGroup();

	_renderWireframeGroup();
}