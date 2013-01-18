#ifndef _OLA_INCLUDE_FORWARDPIPELINE_H__
#define _OLA_INCLUDE_FORWARDPIPELINE_H__

#include "../ola_renderpipeline.h"

class OlaForwardPipeline : public OlaRenderPipeLine
{
public:
	OlaForwardPipeline(OlaRender* render);
	virtual ~OlaForwardPipeline();

	virtual void initialize();
	virtual void release();

	virtual void execute();

protected:

	virtual void _renderGroupedModels();
	virtual void _renderSoildGroup();
	virtual void _renderWireframeGroup();

	void _renderSingleModelGroup(OlaShader* shader,OlaRenderGroup::RenderOpQueue* queue);

	OlaRenderTarget* mShadowmapRT;
	olaMat4 mShadowBiasMatrix;
	olaMat4 mShadowmapTexlProjMatrix;
	
	OlaVFrustum* mLightVFrustum;
};

#endif