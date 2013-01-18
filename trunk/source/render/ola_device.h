#ifndef _OLA_INCLUDE_RENDER_DEVICE_H__
#define _OLA_INCLUDE_RENDER_DEVICE_H__

class OlaVB;
class OlaIB;
class OlaMaterial;
class OlaShader;
class OlaTexture;
class OlaShaderFX;
class OlaRenderOp;
class OlaRenderTarget;
struct OlaGlobalFXConst;

class OlaSwapBuffer
{
public:
	virtual ~OlaSwapBuffer(){};

	virtual void resize(int x,int y) = 0;
	virtual void swapBuffer() = 0;

	virtual void* handle() = 0;
};

class OlaRenderDevice
{
public:
	OlaRenderDevice(){};
	virtual ~OlaRenderDevice(){};

	virtual void init(unsigned int h){};

	virtual OlaVB* spawnVB() = 0;
	virtual OlaIB* spawnIB() = 0;
	virtual OlaTexture* spawnTexture() = 0;
	virtual OlaSwapBuffer* spawnSwapBuffer() = 0;
	virtual OlaShaderFX* spawnShaderFX() = 0;
	virtual OlaRenderTarget* spawnRenderTarget(const char* name) = 0;	

	virtual void setViewport(int x,int y,int w,int h) = 0;
	virtual void clearViewport(float r,float g,float b,float a) = 0;

	virtual void setRenderTarget(OlaRenderTarget* rt) = 0;

	//muse called when switch to rendering on another window,before any device api called
	virtual void setSwapBuffer(OlaSwapBuffer* swb) = 0;

	//switch render state ; set global vars : worldmat viewmat etc
	virtual void bindRenderState(OlaShader* s , OlaGlobalFXConst* shaderconst) = 0;
	virtual void wipeRenderState(OlaShader* s) = 0;

	//update shader vars based on material : texture color etc
	virtual void flushMaterialInfo(OlaMaterial* mat) = 0;

	//draw call
	virtual void drawCall(OlaRenderOp* op, OlaGlobalFXConst* shaderconst) = 0;
};
extern OlaRenderDevice* GetRenderDevice();
extern OlaRenderDevice* SpawnGLRenderDevice();
extern OlaRenderDevice* SpawnDXRenderDevice();

#endif