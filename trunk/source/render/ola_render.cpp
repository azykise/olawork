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

float gBackGroundColor[] = {0.5,0.5,0.5,1.0};

OlaRenderOp::OlaRenderOp(OlaVB* vb,OlaIB* ib,OlaMaterialReference* matref):
mVb(vb),
mIb(ib),
mMatRef(matref),
selectionID(0),
drawmode(OlaRenderParam::DRAWCALL_PRIM_MODE::PRIM_TRILIST)
{
	if (mMatRef)
	{
		mMatRef->addRef();
	}

	worldtrans.Identity();
}

//OlaRenderOp::OlaRenderOp(OlaSubMesh* _sm):
//mVb(_sm->vb()),
//mIb(_sm->ib()),
//mMatRef(new OlaMaterialReference(_sm->orgMaterial())),
//selectionID(0),
//drawmode(OlaRenderParam::DRAWCALL_PRIM_MODE::PRIM_TRILIST)
//{
//	worldtrans.Identity();
//}

OlaRenderOp::~OlaRenderOp()
{
	if (mMatRef)
	{
		mMatRef->delRef();
		mMatRef = 0;
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
}

OlaRender::~OlaRender()
{
	onRelease();
}

void OlaRender::onInitRender(int w,int h,OlaRenderDevice* device)
{
	mScreenW = w;
	mScreenH = h;

	mDevice = device;

	mResourceMng = new OlaResourceMng(device);	

	tViewParams vp;
	memset(&vp,0,sizeof(tViewParams));
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

	mDeviceLost = false;
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
}

OlaVFrustum* OlaRender::getViewFrustum(int index)
{
	return mCurrentFrustum;
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
}

void OlaRender::pushToRender(OlaRenderOp* op)
{
	mPipeline->pushToRender(op);
}


