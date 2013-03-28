#ifndef _OLA_GLRENDER__
#define _OLA_GLRENDER__

#include <map>
#include <vector>
#include <string>

#include "ola_util.h"
#include "math/ola_math.h"
#include "ola_material.h"
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
class OlaRenderScene;
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
	OlaRenderOp();
	OlaRenderOp(OlaSubMesh* _sm);	
	virtual ~OlaRenderOp();
	OlaVB* vb;
	OlaIB* ib;
	
	inline void setMaterial(OlaMaterial* mat)
	{
		if (mMat)
		{
			mMat->delRef();
		}
		mMat = mat;
		mMat->addRef();
	}

	inline OlaMaterial* material(){return mMat;}

	olaMat4 worldtrans;
	OlaRenderParam::DRAWCALL_PRIM_MODE drawmode;
	unsigned int selectionID;

protected:
	OlaMaterial* mMat;
};

class OlaRender
{
public:
	typedef std::map<olastring,OlaRenderTarget*> RenderTargetTable;
	typedef std::vector<OlaLight*> DirectionLightList;

public:
	OlaRender();
	~OlaRender();

	void onInitRender(int w,int h,OlaRenderDevice* device);
	void onRelease();

	OlaVFrustum* getViewFrustum(int index);
	void setCamera(int index,olaVec3& pos,olaVec3& target);
	void getCamera(int index,olaVec3& pos,olaVec3& target);

	void onResize(int w,int h);
	void onRender(float elapse);	

	void setRenderScene(OlaRenderScene* scene);
	void setDirLight(int idx,olaVec3& pos,olaVec3& lookat);

	void pushToRender(CModel* model);
	void pushToRender(OlaRenderOp* op);
	void pushToRender(OlaPrimitive* pri);

	OlaMesh* getMesh(const char* name);
	OlaMaterial* getMaterial(const char* filename,const char* name);

	void exec(void* param0,void* param1,void* param2);

	OlaAssetLoader* getLoader();

	void setJNIAssetMng(AAssetManager* mng);

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
	
	DirectionLightList mDirLights;

	int mScreenW;
	int mScreenH;

	olaMat4 mWorldMatrix;
	olaMat4 mViewMatrix;
	olaMat4 mProjMAtrix;

	OlaRenderPipeLine* mPipeline;

	bool mDeviceLost;

	OlaRenderDevice* mDevice;
};

#endif
