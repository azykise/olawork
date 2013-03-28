#ifndef __OLA__INCLUDE_SCENE__H__
#define __OLA__INCLUDE_SCENE__H__

//#include <vector>

#include "ola_stl.h"

#include "ola_string.h"
#include "math/ola_math.h"
#include "ola_bounding.h"

#include "ola_entity.h"

class CModel;
class OlaRender;
class OlaSkeletonModel;
class OlaLight;
class OlaRenderSceneMng;

class OlaSceneNode
{
public:
	typedef OlaArray<OlaSceneNode*> NodeList;
	typedef OlaArray<ISpatialObj*> AttachedList;
public:
	OlaSceneNode(const char* name,OlaSceneNode* parent){}
	virtual ~OlaSceneNode(){}
	
	virtual olastring& name(){return mName;}

	virtual void addSub(OlaSceneNode* sub){}
	virtual void delSub(OlaSceneNode* sub){}

	virtual void attachObj(ISpatialObj* obj){}
	virtual void detachObj(ISpatialObj* obj){}

	virtual OlaSceneNode* findSubNode(const char* name){return 0;}

	virtual olaAABB* aabb(){return &mAABB;}

	NodeList& subs(){return mSubNodes;}
	AttachedList& objs(){return mAttachedObjs;}
			
	virtual void setTransform(olaQuat& rot){};
	virtual void setTransform(olaVec3& pos){};
	virtual void setTransform(olaMat4& trans){};

	void setTransform(float x,float y,float z){olaVec3 v(x,y,z);setTransform(v);};
	const olaMat4& transform(){return mTransform;};

protected:
	olastring mName;
	OlaSceneNode* mParent;
	AttachedList mAttachedObjs;
	NodeList mSubNodes;

	olaMat4 mTransform;
	olaAABB mAABB;
};

class OlaRenderScene
{
public:
	typedef OlaArray<OlaLight*> LightList;
public:
	OlaRenderScene(const char* name,OlaRenderSceneMng* mng);
	virtual ~OlaRenderScene();

	virtual void initialize();
	virtual void release();
	
	virtual void attachObj(ISpatialObj* obj) = 0;
	virtual void detachObj(ISpatialObj* obj) = 0;	

	virtual OlaSceneNode* root(){return mRoot;}
	virtual OlaSceneNode* findNode(const char* name);

	virtual const LightList* lights(bool all = false);

	olastring& name(){return mName;}

protected:

	OlaRenderSceneMng* mParent;

	olastring mName;

	OlaSceneNode* mRoot;
};

extern OlaRenderScene* newSmallScene(const char* name,OlaRenderSceneMng* mng);

#endif