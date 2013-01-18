#ifndef _OLA_ASSETMANAGER_H__
#define _OLA_ASSETMANAGER_H__

#include <string>
#include <map>

#include "ola_util.h"

#define ASSET_NONAME "asset_noname"

class OlaAsset
{
public:
	OlaAsset();
	~OlaAsset();

	olastring name;
	unsigned int length;
	char* data;	
};

class AAssetManager;
class OlaAssetLoader
{
public:
	typedef std::map<olastring,OlaAsset*> AssetTable;
public:
	static OlaAssetLoader* instance();
	OlaAssetLoader();
	virtual ~OlaAssetLoader();

	virtual void release();

	virtual void clear();

	void setJNIAssetMng(AAssetManager* mng);

	virtual OlaAsset* load(const char* filename , bool cache = true);

protected:

	AssetTable mAssets;
};



#endif
