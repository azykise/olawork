#ifndef __OLA__INCLUDE_SCENE__H__
#define __OLA__INCLUDE_SCENE__H__

//#include <vector>

#include "ola_stl.h"

#include "ola_string.h"
#include "math/ola_math.h"
#include "ola_bounding.h"

class OlaSceneCellImpl;

typedef OlaArray<OlaSceneCellImpl*> CellList;

class OlaSceneNode
{
public:
	typedef OlaArray<OlaSceneNode*> NodeList;
	
public:
	OlaSceneNode(const char* name,OlaSceneNode* parent){}
	virtual ~OlaSceneNode(){}
	
	virtual olastring& name(){return mName;}

	virtual void addSub(OlaSceneNode* sub){}
	virtual void delSub(OlaSceneNode* sub){}

	virtual OlaSceneNode* findSubNode(const char* name){return 0;}

	virtual olaAABB* aabb(){return &mAABB;}

	NodeList& subs(){return mSubNodes;}
	CellList& cells(){return mAttachedCells;}
			
	virtual void setTransform(olaQuat& rot){};
	virtual void setTransform(olaVec3& pos){};
	virtual void setTransform(olaMat4& trans){};

	void setTransform(float x,float y,float z){olaVec3 v(x,y,z);setTransform(v);};
	const olaMat4& transform(){return mTransform;};

protected:
	olastring mName;
	OlaSceneNode* mParent;
	CellList mAttachedCells;
	NodeList mSubNodes;

	olaMat4 mTransform;
	olaAABB mAABB;
};

class OlaSceneBase
{
public:
	OlaSceneBase(const char* name);
	virtual ~OlaSceneBase();

	virtual void initialize();
	virtual void release();
	
	virtual void attachCell(OlaSceneCellImpl* obj) = 0;
	virtual void detachCell(OlaSceneCellImpl* obj) = 0;	

	virtual CellList& visibleCells() = 0;

	virtual OlaSceneNode* root(){return mRoot;}
	virtual OlaSceneNode* findNode(const char* name);

	olastring& name(){return mName;}

protected:

	olastring mName;

	OlaSceneNode* mRoot;
};

//extern OlaSceneBase* newSmallScene(const char* name,OlaRenderSceneMng* mng);

#endif