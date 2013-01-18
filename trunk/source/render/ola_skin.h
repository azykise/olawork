#ifndef _OLA_SKIN_H__
#define _OLA_SKIN_H__

#include <vector>

#include "ola_assetmng.h"
#include "ola_geobuffer.h"

#include <vector>
#include <map>

#include "ola_ase.h"
#include "ola_string.h"

class OlaMesh;
class OlaSubMesh;
class OlaSkeleton;
class OlaActionChannelSet;

class OlaSkin
{
public:
	OlaSkin(OlaMesh* org_mesh);
	virtual ~OlaSkin();

	bool attachSkeleton(OlaSkeleton* skeleton);

	void updateCPUSkin(OlaActionChannelSet* channel_set);

	OlaMesh* mesh(){return mSkinMesh;}

protected:
	OlaSkeleton* mSkeleton;
	OlaMesh* mSkinMesh;
	OlaMesh* mSrcMesh;

	int mBoneIDTable[128];
};

#endif