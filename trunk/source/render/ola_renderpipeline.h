#ifndef _OLA_INCLUDE_RENDER_PIPELINE_H__
#define _OLA_INCLUDE_RENDER_PIPELINE_H__

class OlaMaterial;
class OlaSubMesh;
class OlaRenderOp;
class OlaShader;
class OlaResourceMng;
class OlaRenderQueue;
class OlaRenderGroup;
class OlaRender;
class OlaLight;
class OlaScreenQuad;
class OlaRenderTarget;
class OlaRenderScene;
class OlaLight;
class OlaVFrustum;
class CModel;

#include "ola_util.h"
#include "math/ola_math.h"
#include "ola_renderqueue.h"
#include "ola_fxconst.h"

class OlaPipeLineParam
{
public:
	enum NOTIFY_TYPE
	{
		NOTIFY_RESIZE,
		NOTIFY_TYPE_END,
	};
};

//先将render strategy 和 pipeline 合为一体 即控制流程又控制方式
class OlaRenderPipeLine
{
public:
	enum MATRIX_TYPE
	{
		VIEW_MATRIX,
		PROJ_MATRIX,
	};

	typedef std::stack<olaMat4*> MatrixStack;
public:
	OlaRenderPipeLine(OlaRender* render);
	virtual ~OlaRenderPipeLine(){};

	virtual void initialize(){};
	virtual void release(){};

	virtual void notify(OlaPipeLineParam::NOTIFY_TYPE type,void* var0,void* var1){};

	virtual void pushToRender(CModel* model);
	virtual void pushToRender(OlaRenderOp* op);
	virtual void pushToRender(OlaPrimitive* pri);

	virtual void pushLight(OlaLight* l);

	virtual void execute(){};

	virtual void resize(int w,int h){};

	virtual void setCurrentScene(OlaRenderScene* s){mCurrentScene = s;}

	void setMatrix(OlaRenderPipeLine::MATRIX_TYPE type,olaMat4& mat);
	void pushMatrix(OlaRenderPipeLine::MATRIX_TYPE type,olaMat4& mat);
	void popMatrix(OlaRenderPipeLine::MATRIX_TYPE type);

	virtual void setRenderTarget(OlaRenderTarget* rt);	

protected:

	OlaRender* mRender;
	OlaRenderQueue* mRenderQueue;

	olaMat4 mWorldMatrix;
	olaMat4 mViewMatrix;
	olaMat4 mProjMatrix;

	MatrixStack mViewMatrixs;
	MatrixStack mProjMatrixs;

	OlaGlobalFXConst mGlobalShaderConst;

	OlaRenderScene* mCurrentScene;
	OlaRenderTarget* mGLOrgRT;

	OlaArray<OlaLight*> mLights;
};

#endif