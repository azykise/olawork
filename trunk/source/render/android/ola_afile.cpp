#include <stdlib.h>

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "ola_afile.h"

#include "ola_util.h"

AAssetManager* gAssetMng = 0;

OlaAFile::OlaAFile():
mFile(0)
{

}

OlaAFile::~OlaAFile()
{
	close();
}
bool OlaAFile::open(const char* filename)
{
	if(gAssetMng)
    	mFile = AAssetManager_open(gAssetMng, filename,AASSET_MODE_UNKNOWN);

	if(mFile)
		lg("open file %s done \n",filename);
	else
		lg("open file %s failed \n",filename);

	return mFile != 0;
}

int OlaAFile::read(void* buffer,int size,int num)
{
	if(mFile)
	{
		return AAsset_read(mFile,buffer,size * num);
	}
	return 0;
}

int OlaAFile::seek(long offset,int origin)
{
	if(mFile)
	{
		return AAsset_seek(mFile,offset,origin);
	}
	return 0;
}

void OlaAFile::close()
{
	if(mFile)
	{
		AAsset_close(mFile);
		mFile = 0;
	}
}

int OlaAFile::getc()
{
	if(mFile)
	{
		int c = 0;
		AAsset_read(mFile, &c, 1);
		return c;
	}
	return 0;
}

long OlaAFile::tell()
{
	if(mFile)
	{
		off_t asset_l = AAsset_getLength(mFile);
		off_t asset_r = AAsset_getRemainingLength(mFile);
		return asset_l - asset_r;
	}
	return 0;
}

int OlaAFile::len()
{
	if(mFile)
	{
		return AAsset_getLength(mFile);
	}
	return 0;
}
