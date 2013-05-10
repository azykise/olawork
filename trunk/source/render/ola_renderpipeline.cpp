#include "ola_render.h"
#include "ola_light.h"
#include "model.h"

#include "ola_renderpipeline.h"

#include "ola_material.h"
#include "ola_mesh.h"
#include "ola_resourcemng.h"

#include "ola_renderqueue.h"
#include "ola_postprocess.h"

#include "ola_device.h"

#define lgGLError(x) 	if(glGetError() != GL_NO_ERROR){lg(x);}

OlaRenderPipeLine::OlaRenderPipeLine(OlaRender* render):
mRender(render),mRenderQueue(0),mGLOrgRT(0),mCurrentScene(0)
{
	mWorldMatrix.Identity();
	mViewMatrix.Identity();
	mProjMatrix.Identity();
}

void OlaRenderPipeLine::pushToRender(CModel* model)
{
	CModel::RenderOpList& op_list = model->renderops();
	CModel::RenderOpList::iterator op_i = op_list.begin();
	while(op_i != op_list.end())
	{
		OlaRenderOp* op = (*op_i);

		op->worldtrans = model->getWorldTransform();

		mRenderQueue->pushToRener(op);
		
		op_i++;
	}
}

void OlaRenderPipeLine::pushToRender(OlaRenderOp* op)
{
	mRenderQueue->pushToRener(op);
}

void OlaRenderPipeLine::pushToRender(OlaPrimitive* pri)
{
	OlaPrimitiveGroup* prim_group = mRenderQueue->primGroup();
	if(prim_group)
		prim_group->prims().push_back(pri);
}

void OlaRenderPipeLine::setRenderTarget(OlaRenderTarget* rt)
{
	GetRenderDevice()->setRenderTarget(rt);
}

void OlaRenderPipeLine::setMatrix(OlaRenderPipeLine::MATRIX_TYPE type,olaMat4& mat)
{
	switch(type)
	{
		case VIEW_MATRIX:
			mViewMatrix = mat;
			break;
		case PROJ_MATRIX:
			mProjMatrix = mat;
			break;;
	}
}
void OlaRenderPipeLine::pushMatrix(OlaRenderPipeLine::MATRIX_TYPE type,olaMat4& mat)
{
	switch(type)
	{
		case VIEW_MATRIX:
			mViewMatrixs.push(&mat);
			break;
		case PROJ_MATRIX:
			mProjMatrixs.push(&mat);
			break;;
	}
}
void OlaRenderPipeLine::popMatrix(OlaRenderPipeLine::MATRIX_TYPE type)
{
	switch(type)
	{
		case VIEW_MATRIX:
			mViewMatrixs.pop();
			break;
		case PROJ_MATRIX:
			mProjMatrixs.pop();
			break;;
	}
}

void OlaRenderPipeLine::pushLight( OlaLight* l )
{
	mLights.push_back(l);
}









