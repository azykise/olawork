#ifndef _OLA_DML_FILEPARSER_
#define _OLA_DML_FILEPARSER_

#include "../ola_util.h"

class OlaMesh;
class OlaSubMesh;
class OlaMaterial;
class OlaMaterialPool;

struct tDmlFileInfo
{
	olastring DmlFullname;
	olastring ASEFullname;
	OlaArray<olastring> MeshMatsFullname;
};

struct tDmlResult
{
	OlaMesh* Mesh;
	OlaArray<OlaMaterial*> MeshMats;
};

class OlaDMLParser
{
public:
	OlaDMLParser(OlaMaterialPool* matpool = 0);

	virtual bool parseDMLInfoFromData(const char* data,int len,tDmlFileInfo* outDmlInfo);	

	virtual bool fillDML(tDmlFileInfo* dmlInfo,tDmlResult* dml);
protected:
	OlaMaterialPool* mMaterialPool;
};

class OlaMeshParser
{
public:
	typedef OlaArray<int> MeshFaceList;
public:
	virtual bool parseMeshFromData(const char* data,int len,OlaMesh* outMesh);
};

#endif