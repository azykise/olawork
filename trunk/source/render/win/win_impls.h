#pragma once

#include <vector>

#include "../../interface/ola_engine.h"

#include "../math/ola_math.h"

#include "../ola_scenemng.h"

#include "../ola_render.h"

#include "../ola_material.h"
#include "../model.h"
#include "../ola_primitive.h"
#include "../ola_skeleton.h"
#include "../ola_light.h"
#include "../ola_scene.h"

class OlaPrimitiveImpl : public ola::IPrimitive
{
public:
	OlaPrimitiveImpl(OlaPrimitive* prim,OlaRenderSceneMng* scene_mng):
	mPrimitive(prim),
	mSceneMng(scene_mng)
	{
	};

	virtual ~OlaPrimitiveImpl()
	{
		if (mPrimitive)
		{
			delete mPrimitive;
			mPrimitive = 0;
		}
	};

	virtual unsigned long long getID(){return mPrimitive->id();};

	virtual ola::PrimitiveType getType() {return ola::PMT_LINE;};

	virtual void setVertex(float* vec3s,int length) {mPrimitive->setVertex(vec3s,length);};

	virtual void setColor(float r,float g,float b,float a) {mPrimitive->setColor(r,g,b,a);};

	virtual void setIndex(int* idxs,int length) {mPrimitive->setIndex(idxs,length);};

	OlaPrimitive* mPrimitive;
	OlaRenderSceneMng* mSceneMng;
};


class OlaMaterialImpl : public ola::IMaterial
{
public:
	OlaMaterialImpl(OlaMaterial* mat,OlaResourceMng* res_mng):
	mMaterial(mat),
	mResourceMng(res_mng)
	{
		mMaterial->addRef();
	}

	virtual~ OlaMaterialImpl()
	{
		mMaterial->delRef();
	}

	virtual const char* name() {return mMaterial->name().c_str();}

	virtual const char* getResourceFilename(){return mMaterial->filename().c_str();}

	virtual void reload();

	OlaResourceMng* mResourceMng;
	OlaMaterial* mMaterial;
};

class OlaGeometryImpl : public ola::IGeometry
{
public:
	OlaGeometryImpl(CModel* model,OlaResourceMng* res_mng);
	virtual ~OlaGeometryImpl();

	virtual int submeshNum(){ return mSubMaterials.size(); };

	virtual ola::IMaterial* submeshMaterial(int idx) { return mSubMaterials[idx];}

	virtual void reload(){};

	virtual const char* getResourceFilename(){return mModel->name();}

	OlaResourceMng* mResourceMng;
	CModel* mModel;
	std::vector<OlaMaterialImpl*> mSubMaterials;
};

class OlaSkeletonImpl;
class OlaBoneImpl : public ola::IBone
{
public:
	OlaBoneImpl(OlaBone* bone,OlaSkeletonImpl* skl);

	virtual ~OlaBoneImpl();

	virtual const char* name();

	virtual int id();		

	virtual float* matrix(); //4x4 world space matrix

	virtual ola::IBone* parent(){return mParent;};

	virtual int subNum(){return (int)mChildren.size();}
	virtual ola::IBone* sub(int idx);

	OlaBone* mBone;
	OlaBoneImpl* mParent;
	std::vector<OlaBoneImpl*> mChildren;
	OlaSkeletonImpl* mSkeleton;
};

class OlaSkeletonImpl : public ola::ISkeleton
{
public:
	OlaSkeletonImpl(OlaSkeletonModel* skl);
	virtual ~OlaSkeletonImpl();

	virtual const char* getResourceFilename();	

	virtual void reload(){};

	virtual int boneNum();

	virtual ola::IBone* bone(int id);

	virtual ola::IBone* root();

	void _buildSkeleton();

	OlaSkeletonModel* mParent;
	OlaBoneImpl* mRoot;
	std::vector<OlaBoneImpl*> mBones;
};

class OlaRenderableImpl : public ola::IRenderable
{
public:
	typedef std::vector<OlaRenderOp*> RenderOpList;
public:
	OlaRenderableImpl();
	virtual ~OlaRenderableImpl();

	void clear();

	RenderOpList mRenderOps;
};

class OlaSceneImpl;
class OlaSceneEntryImpl : public ola::ISceneEntry
{
public:
	enum ENTRY_TYPE
	{
		ENTRY_LIGHT = 0,
		ENTRY_MODEL,
		ENTRY_TYPE_END,
	};
public:
	OlaSceneEntryImpl(){};
	virtual ~OlaSceneEntryImpl(){};

	virtual ENTRY_TYPE type() = 0;

	virtual void setScene(OlaSceneImpl* scene){mScene = scene;}
	virtual ola::IScene* scene();
protected:
	OlaSceneImpl* mScene;
};

class OlaStaticModelImpl : public ola::IStaticModel
{
public:
	OlaStaticModelImpl(CModel* model,OlaResourceMng* res_mng):
	mGeometry(0),
	mResourceMng(res_mng)
	{
		mGeometry = new OlaGeometryImpl(model,res_mng);
	}

	virtual ~OlaStaticModelImpl()
	{
		delete mGeometry;
	}

	virtual ola::IGeometry* geometry(){return mGeometry;}

	virtual ola::ITransform* transform(){return 0;}

	virtual ola::tAABB* aabb();	

	OlaGeometryImpl* mGeometry;
	OlaResourceMng* mResourceMng;

	ola::tAABB mAABB;
};

class OlaCharacterImpl : public ola::ICharacter
{
public:
	
	OlaCharacterImpl(OlaSkeletonModel* model);

	virtual ~OlaCharacterImpl();

	virtual ola::tAABB* aabb();

	virtual void setPassedTime(float t);

	virtual void update(float elasped);	

	virtual void setAction(int channel,const char* action_name);

	virtual int getActionFrameNumber(int channel);

	virtual float getActionTime(int channel);

	virtual const char* getActionName(int channel);

	virtual ola::IRenderable* renderer();

	virtual ola::ISkeleton* skeleton(){return mSkeleton;};

	virtual ola::ITransform* transform(){return 0;}

	OlaSkeletonModel* mSkeletonModel;
	OlaRenderableImpl* mRenderer;
	OlaSkeletonImpl* mSkeleton;
};

class OlaLightTransform;
class OlaSceneEntryLight;
class OlaLightImpl : public ola::ILight
{
public:
	OlaLightImpl(OlaLight* l);

	virtual ~OlaLightImpl();

	virtual ola::tAABB* aabb() {return 0;}

	virtual ola::ITransform* transform();

	virtual void setTargetPos(float x,float y,float z);
	virtual void getTargetPos(ola::tVec3* out_pos);

	virtual ola::ISceneEntry* entry();

	OlaLight* mLight;
	OlaLightTransform* mTransform;
	OlaSceneEntryLight* mSceneEntry;
};



class OlaSceneImpl : public ola::IScene
{
public:
	OlaSceneImpl(OlaRenderScene* scene,OlaRenderSceneMng* mng);
	virtual ~OlaSceneImpl();

	virtual const char* name();
	virtual void attachEntry(ola::ISceneEntry* entry);

	OlaRenderSceneMng* mSceneMng;
	OlaRenderScene* mScene;
};







