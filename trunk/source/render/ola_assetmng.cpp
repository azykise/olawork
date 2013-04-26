#include "ola_assetmng.h"
#include "ola_file.h"

#if defined __ANDROID_API__
#include "ola_afile.h"
extern AAssetManager* gAssetMng;
#elif defined __APPLE__
#include "ola_ifile.h"
#endif

OlaAsset::OlaAsset():
data(0),length(0),
name(ASSET_NONAME)
{

}

OlaAsset::~OlaAsset()
{
	if(data)
	{
		free(data);
	}
}

static OlaAssetLoader* gAssetLoader = 0;

OlaAssetLoader* OlaAssetLoader::instance()
{
	return gAssetLoader;
}

OlaAssetLoader::OlaAssetLoader()
{
	gAssetLoader = this;
}

OlaAssetLoader::~OlaAssetLoader()
{
	gAssetLoader = 0;
}

void OlaAssetLoader::release()
{
	clear();
}

void OlaAssetLoader::clear()
{
	AssetTable::iterator i = mAssets.begin();
	while(i != mAssets.end())
	{
		delete i->second;
		++i;
	}
	mAssets.clear();

}


void OlaAssetLoader::setJNIAssetMng(AAssetManager* mng)
{
#if defined __ANDROID_API__
	gAssetMng = mng;
	if(gAssetMng)
		lg("Jni asset mng inited \n");
#endif
}

OlaAsset* OlaAssetLoader::load(const char* filename , bool cache)
{
	OlaAsset* ola_asset = new OlaAsset();
	load(filename,ola_asset,cache);
	return ola_asset;
}

bool OlaAssetLoader::load( const char* filename,OlaAsset* ola_asset,bool cache /*= true*/ )
{
	olastring asset_name(filename);

    char *pResult = NULL;
    long iLen = 0;

	OlaFile* asset_file = 0;

#if defined WIN32
	asset_file = new OlaWFile();
#elif defined __ANDROID_API__
	asset_file = new OlaAFile();
#elif defined __APPLE__
	asset_file = new OlaIFile();
#endif	

	if(!asset_file->open(filename))
	{
		lg("asset: %s open failed!!!!",filename);
		delete asset_file;
		return 0;
	}
	iLen = asset_file->len();
	lg("file %s len: %d\n",filename,iLen);

	pResult = (char*)malloc(iLen + 1);
	asset_file->read(pResult,sizeof(char),iLen);
	pResult[iLen] = '\0';
	asset_file->close();
	delete asset_file;
	
	ola_asset->length = iLen + 1;
	ola_asset->data =(char*)malloc(ola_asset->length); 
	memcpy(ola_asset->data,pResult,ola_asset->length);
	free(pResult);  	

	AssetTable::iterator i = mAssets.find(asset_name);
	if( i != mAssets.end())
	{
		delete i->second;
		mAssets.erase(i);
	}

	if(cache)
		mAssets[asset_name] = ola_asset;

	return ola_asset;
}

bool LoadAssetFile(const char* filename,OlaAsset* outAsset)
{
	OlaAssetLoader::instance()->load(filename,outAsset,false);
	return true;
}
