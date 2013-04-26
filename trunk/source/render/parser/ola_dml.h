#ifndef _OLA_DML_FILEPARSER_
#define _OLA_DML_FILEPARSER_

#include "ola_parser.h"

class OlaMATParser;
class OlaDMLParser
{
public:
	OlaDMLParser(tResourcePools* ps = 0);

	virtual ~OlaDMLParser();

	virtual bool parseDMLInfoFromData(const char* data,int len,tDmlFileInfo* outDmlInfo);	

	virtual bool fillDML(tDmlFileInfo* dmlInfo,tDmlResult* dml);
protected:
	OlaMATParser* mMATParser;
	tResourcePools* mPools;
};

class OlaMeshParser
{
public:
	typedef OlaArray<int> MeshFaceList;
public:
	virtual bool parseMeshFromData(const char* data,int len,OlaMesh* outMesh);
};

#endif