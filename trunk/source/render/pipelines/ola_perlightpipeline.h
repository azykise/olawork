#ifndef _OLA_INCLUDE_PERLIGHTPIPELINE_H__
#define _OLA_INCLUDE_PERLIGHTPIPELINE_H__

#include "ola_forwardpipeline.h"

class OlaScreenQuadChain;
class OlaPerLightPipeline : public OlaForwardPipeline
{
public:
	OlaPerLightPipeline(OlaRender* render);
	virtual ~OlaPerLightPipeline();

	virtual void initialize();
	virtual void release();

	virtual void resize(int w,int h);

	virtual void execute();
protected:

	virtual void _renderShadowMap();

    void _renderScreenFinalQuad();
    
	OlaScreenQuadChain* mChain;	
    
	OlaRenderTarget* mSceneInputRT;    
	OlaRenderTarget* mPerLightRT;

	OlaScreenQuad*	 mSceneFinalQuad;
	OlaRenderOp*	 mFinalQuadRenderOp;	
};


#endif