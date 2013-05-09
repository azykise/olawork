#include "ola_render.h"
#include "ola_util.h"

#include "ola_primitive.h"
#include "ola_mesh.h"
#include "ola_assetmng.h"
#include "ola_resourcemng.h"
#include "ola_light.h"
#include "ola_postprocess.h"

#include "ola_renderpipeline.h"
#include "pipelines/ola_forwardpipeline.h"
#include "pipelines/ola_perlightpipeline.h"
#include "pipelines/ola_selectionpipeline.h"

#include "model.h"

#define lgGLError(x) 	if(glGetError() != GL_NO_ERROR){lg(x);}

float gBackGroundColor[] = {0.5,0.5,0.5,1.0};

OlaRenderOp::OlaRenderOp():
vb(0),
ib(0),
mMat(0),
selectionID(0),
drawmode(OlaRenderParam::DRAWCALL_PRIM_MODE::PRIM_TRILIST)
{
	worldtrans.Identity();
}

OlaRenderOp::OlaRenderOp(OlaSubMesh* _sm):
vb(_sm->vb()),
ib(_sm->ib()),
mMat(0),
selectionID(0),
drawmode(OlaRenderParam::DRAWCALL_PRIM_MODE::PRIM_TRILIST)
{
	worldtrans.Identity();
}

OlaRenderOp::~OlaRenderOp()
{
	if (mMat)
	{
		mMat->delRef();
	}
}

OlaRender::OlaRender():
mCurrentFrustum(0),
mTotalTris(0),
mDeviceLost(true),
mPipeline(0),
mDevice(0)
{
	olaMath::Init();	
	mWorldMatrix.Identity();
	mViewMatrix.Identity();
	mProjMAtrix.Identity();
}

OlaRender::~OlaRender()
{
	onRelease();
}

void OlaRender::onInitRender(int w,int h,OlaRenderDevice* device)
{
    //glClearColor(gBackGroundColor[0],gBackGroundColor[1],gBackGroundColor[2],gBackGroundColor[3]);
	//glViewport(0,0,w,h);
	mScreenW = w;
	mScreenH = h;

	mDevice = device;

	mResourceMng = new OlaResourceMng(device);	

	tViewParam vp;
	memset(&vp,0,sizeof(tViewParam));
	vp.aspect = (float)mScreenW/(float)mScreenH;
	vp.far_dist = 1000.f;
	vp.near_dist = 1.0f;
	vp.fovy = olaMath::M_DEG2RAD * 45.0f;
	vp.lookat[0] = 0; vp.lookat[1] = 0; vp.lookat[2] = 0;
	vp.pos[0] = 0; vp.pos[1] = 0; vp.pos[2] = 5;

	mCurrentFrustum = new OlaVFrustum();
	mCurrentFrustum->fromViewParam(&vp);

	mPipeline = new OlaForwardPipeline(this);
    //mPipeline = new OlaPerLightPipeline(this);
	mPipeline->initialize();

	mDirLights.push_back(new OlaLight());

	mDeviceLost = false;

//	mPipeline->pushLights(mDirLights[0]);

	lg("Render Inited\n");
}

void OlaRender::exec(void* param0,void* param1,void* param2)
{

}

void OlaRender::onRelease()
{
	if(mPipeline)
	{
		mPipeline->release();
		delete mPipeline;
		mPipeline = 0;
	}

	if(mResourceMng)
	{
		mResourceMng->clear();
		delete mResourceMng;
		mResourceMng = 0;
	}

	if(mCurrentFrustum)
	{
		delete mCurrentFrustum;
		mCurrentFrustum = 0;
	}

	DirectionLightList::iterator light_i = mDirLights.begin();
	while(light_i != mDirLights.end())
	{
		delete *light_i;
		light_i++;
	}
	mDirLights.clear();
	
	lg("OlaRender Released !!!!.............................. \n");
}

void OlaRender::setCamera(int index,olaVec3& pos,olaVec3& target)
{
	mCurrentFrustum->setLookPt(target);
	mCurrentFrustum->setEyePt(pos);
}

void OlaRender::getCamera(int index,olaVec3& pos,olaVec3& target)
{
	pos = mCurrentFrustum->getEyePt();
	target = mCurrentFrustum->getLookPt();
}

OlaVFrustum* OlaRender::getViewFrustum(int index)
{
	return mCurrentFrustum;
}

void OlaRender::setDirLight(int idx,olaVec3& pos,olaVec3& lookat)
{
	if(idx >= mDirLights.size())
		return;

	mDirLights[idx]->setLookAtPT(lookat);
	mDirLights[idx]->position(pos);
}

void OlaRender::onResize(int w,int h)
{
	mDeviceLost = true;
	
	mScreenW = w;
	mScreenH = h;
	
	mCurrentFrustum->setAspect((float)w/(float)h);

	mPipeline->resize(w,h);

	mDeviceLost = false;
}

#include <iostream>
void OlaRender::onRender(float elapse)
{
	if(mDeviceLost)
		return;

	mTotalTris = 0;		

	mPipeline->setMatrix(OlaRenderPipeLine::VIEW_MATRIX,mCurrentFrustum->getViewMatrix());
	mPipeline->setMatrix(OlaRenderPipeLine::PROJ_MATRIX,mCurrentFrustum->getProjMatrix());

	mPipeline->execute();

	static int total_tri = mTotalTris;

	if (total_tri != mTotalTris)
	{
		total_tri = mTotalTris;
		std::cout<<total_tri<<std::endl;
	}
}

OlaMesh* OlaRender::getMesh(const char* name)
{
	return mResourceMng->getMesh(name);
}

OlaMaterial* OlaRender::getMaterial(const char* filename,const char* name)
{
	return mResourceMng->getMaterial(filename,name);
}

OlaAssetLoader* OlaRender::getLoader()
{
	return mResourceMng->getLoader();
}

void OlaRender::setJNIAssetMng(AAssetManager* mng)
{
	mResourceMng->getLoader()->setJNIAssetMng(mng);
}

void OlaRender::pushToRender(CModel* model)
{
	mPipeline->pushToRender(model);
}

void OlaRender::pushToRender(OlaRenderOp* op)
{
	mPipeline->pushToRender(op);
}

void OlaRender::pushToRender(OlaPrimitive* pri)
{
	mPipeline->pushToRender(pri);
}

void OlaRender::setRenderScene( OlaRenderScene* scene )
{
	mPipeline->setCurrentScene(scene);
}


