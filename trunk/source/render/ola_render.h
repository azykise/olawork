#ifndef _OLA_GLRENDER__
#define _OLA_GLRENDER__

#include <map>
#include <vector>
#include <string>

#include "ola_util.h"
#include "math/ola_math.h"
#include "ola_material_ref.h"
#include "ola_camera.h"

class AAssetManager;

class CModel;
class OlaSubMesh;
class OlaVB;
class OlaIB;
class OlaMaterial;
class OlaMesh;
class OlaAsset;
class OlaResourceMng;
class OlaMaterial;
class OlaAssetLoader;
class OlaLight;
class OlaRenderTarget;
class OlaScreenQuad;
class OlaScreenQuadChain;
class OlaRenderPipeLine;
class OlaPrimitive;
class OlaSceneBase;
class OlaRenderDevice;

class OlaRenderParam
{
public:
	enum SYSTEM_CAMERA_TYPE
	{
		SCENE_CAMERA = 0,
		QUAD_CAMERA,
		SYSTEM_CAMERA_TYPE_END,
	};

	enum DRAWCALL_PRIM_MODE
	{
		PRIM_LINE,
		PRIM_TRILIST,
	};
};

//一个op只有一个网格
class OlaRenderOp
{
public:
	OlaRenderOp(OlaVB* vb,OlaIB* ib,OlaMaterialReference* matref);
	//OlaRenderOp(OlaSubMesh* _sm);	

	virtual ~OlaRenderOp();

	inline OlaVB* vb(){return mVb;}
	inline OlaIB* ib(){return mIb;}	

	inline OlaMaterial* material(){return mMatRef->material();}

	olaMat4 worldtrans;
	OlaRenderParam::DRAWCALL_PRIM_MODE drawmode;
	unsigned int selectionID;

protected:
	OlaVB* mVb;
	OlaIB* mIb;
	OlaMaterialReference* mMatRef;
};

class OlaRender
{
public:
	OlaRender();
	~OlaRender();

	void onInitRender(int w,int h,OlaRenderDevice* device);
	void onRelease();

	OlaVFrustum* getViewFrustum(int index);

	void onResize(int w,int h);
	void onRender(float elapse);	

	void pushToRender(OlaRenderOp* op);

	void exec(void* param0,void* param1,void* param2);

	int getTotalTris(){return mTotalTris;}
    void setTotalTris(int t){mTotalTris = t;};

	OlaResourceMng* getResourceMng(){return mResourceMng;}

	int getScreenW(){return mScreenW;}
	int getScreenH(){return mScreenH;}

	inline OlaRenderDevice* device(){return mDevice;}
protected:

	int mTotalTris;
    
	OlaResourceMng* mResourceMng;

	OlaVFrustum* mCurrentFrustum;

	int mScreenW;
	int mScreenH;

	OlaRenderPipeLine* mPipeline;

	bool mDeviceLost;

	OlaRenderDevice* mDevice;
};

#endif
