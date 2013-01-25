#ifndef _OLA_INCLUDE_OPENGL_RENDER_DEVICE_H__
#define _OLA_INCLUDE_OPENGL_RENDER_DEVICE_H__

#include "../ola_device.h"

class OlaGLDevice : public OlaRenderDevice
{
public:
	OlaGLDevice();

	virtual ~OlaGLDevice();

	virtual OlaVB* spawnVB();
	virtual OlaIB* spawnIB();

	virtual OlaRenderTarget* spawnRenderTarget( const char* name );
	virtual OlaTexture* spawnTexture();
	virtual OlaSwapBuffer* spawnSwapBuffer();

	virtual OlaShaderFX* spawnShaderFX();
	
	virtual void setRenderTarget( OlaRenderTarget* rt );
	virtual void setSwapBuffer(OlaSwapBuffer* swb);

	virtual void bindRenderState( OlaShader* s , OlaGlobalFXConst* shaderconst);
	virtual void wipeRenderState( OlaShader* s );

	virtual void flushMaterialInfo( OlaMaterial* mat );

	virtual void drawCall( OlaRenderOp* op , OlaGlobalFXConst* shaderconst);

	inline void* glDisplay(){return mEGLDisplay;}
	inline void* glContext(){return mEGLContext;}
	inline void** glConfigs(){return mEGLConfigs;}

	virtual void init();

	virtual void setViewport( int x,int y,int w,int h );

	virtual void clearViewport( float r,float g,float b,float a );
protected:

	void _createGLRenderContext(unsigned int h);

	void* mEGLSurface;
	void* mEGLDisplay;	
	void* mEGLContext;
	void* mEGLConfigs[1];
};

#endif