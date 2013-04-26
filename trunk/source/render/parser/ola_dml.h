#ifndef _OLA_DML_FILEPARSER_
#define _OLA_DML_FILEPARSER_

#include "ola_parser.h"

class OlaDMLParser
{
public:
	OlaDMLParser(OlaMeshPool* meshpool = 0,OlaMaterialPool* matpool = 0);

	virtual bool parseDMLInfoFromData(const char* data,int len,tDmlFileInfo* outDmlInfo);	

	virtual bool fillDML(tDmlFileInfo* dmlInfo,tDmlResult* dml);
protected:
	OlaMeshPool* mMeshPool;
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