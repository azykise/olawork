#ifndef __OLA__SCENE__MANAGER__H__
#define __OLA__SCENE__MANAGER__H__

#include <vector>
#include <map>
#include "ola_stl.h"

#include "ola_string.h"
#include "math/ola_math.h"

#include "ola_transobj.h"
#include "ola_scene.h"

class CModel;
class OlaAsset;
class OlaRender;
class OlaPrimitive;
class OlaSkeleton;
class OlaSkeletonModel;
class OlaLight;
class OlaRenderSceneMng;

class OlaRenderSceneMng
{
public:
	typedef std::vector<CModel*> ModelList;
	typedef std::vector<OlaPrimitive*> PrimitiveList;
	typedef std::vector<OlaSkeletonModel*> SkeletonModelList;	
	typedef OlaArray<OlaRenderScene*> SceneList;

	enum SCENE_TYPE
	{
		SCENE_SMALL = 0,
		SCENE_OCTREE,
		SCENE_BSP,
	};

public:
	OlaRenderSceneMng(OlaRender* render);

	~OlaRenderSceneMng();

	void initialize();
	void clear();

	void loadScene(const char* filename);

	void setRenderScene(OlaRenderScene* scene);
	OlaRenderScene* scene(){return mCurrentScene;};
	OlaRenderScene* scene(const char* name);

	OlaRenderScene* createScene(const char* name,SCENE_TYPE t);

	CModel* loadModelFromDML(const char* filename);

	OlaSkeletonModel* loadSkeletonModelFromCHR(const char* filename);

	ModelList& models() {return mModels;};

	PrimitiveList& prims(){return mPrims;}

	SkeletonModelList& charactor(){return mSkeletonModels;}	

	OlaSkeleton* girlskeleton(){return mGirlSkeleton;}

	bool draw_all;
public:
	ModelList mModels;
	PrimitiveList mPrims;
	SkeletonModelList mSkeletonModels;
	
	OlaRenderScene* mCurrentScene;
	OlaRenderScene* mScene0;

	OlaSkeleton* mGirlSkeleton;
	OlaRender* mRender;
};

#endif