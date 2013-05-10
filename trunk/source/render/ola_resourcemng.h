#ifndef _OLA_RESOURCE_MANAGER_H__
#define _OLA_RESOURCE_MANAGER_H__

#include "ola_util.h"
#include "ola_string.h"
#include "ola_resource.h"

#include "ola_mesh.h"
#include "ola_material.h"
#include "ola_texture.h"
#include "ola_shaderfx.h"

#include "parser/ola_parser.h"

class OlaAssetLoader;
class OlaSkeleton;
class OlaActionResource;
class OlaRenderDevice;

class OlaResourceMng
{
public:
	struct RESOURCE_TRUNK_TYPE
	{
		enum TRUNK_TYPE
		{
			RESOURCE_TRUNK = 0,
			SYSTEM_TRUNK,
			TRUNK_END,
		};
	};
	typedef std::map<olastring,OlaMesh*>		MeshTable;
	typedef std::map<olastring,OlaMaterial*>	MaterialTable;
	typedef std::map<olastring,OlaTexture*>		TextureTable;

	typedef std::map<olastring,OlaSkeleton*>	SkeletonTable;
	typedef std::map<olastring,OlaActionResource*> ActionResourceTable;

	typedef std::map<olastring,OlaShader*>	    ShaderTable;
public:
	OlaResourceMng(OlaRenderDevice* device);
	~OlaResourceMng();

	static OlaResourceMng* instance();

	static olastring FilePathToAssetPath(const olastring& filepath);

	void clear();

	OlaMesh* getMesh(const char* filename);
	OlaTexture* getTexture(const char* filename);
	OlaMaterial* getMaterial(const char* filename,const char* name);

	bool reloadMaterial(const char* filename,const char* name);

	OlaAssetLoader* getLoader(){return mLoader;}

	OlaMaterial* getPrimitiveMaterial();
	OlaMaterial* getShadowcastMaterial();

	OlaSkeleton* getSkeleton(const char* name,const char* filename);
	OlaActionResource* getActionResource(const char* filename);

	void clearResourceCache(const char* res_type);
	int getResourceNum(const char* res_type);

	tResourcePools* pools(){return mPools;};

protected:	

	MeshTable		mMeshs;
	TextureTable	mTextures;
	SkeletonTable   mSkeletons;

	ShaderTable		mResourceShaders;
	ShaderTable		mSystemShaders;

	MaterialTable	mResourceMats;
	MaterialTable	mSystemMats;

	ActionResourceTable mActionResources;

	OlaAssetLoader* mLoader;
	OlaRenderDevice* mDevice;

	tResourcePools* mPools;
};

#endif