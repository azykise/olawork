#ifndef _OLA_MAT_FILEPARSER_
#define _OLA_MAT_FILEPARSER_

#include "ola_parser.h"

class OlaMATParser
{
public:
	enum MATFILL_MODE
	{
		CACHED_FILLMAT,
		RELOAD_FILLMAT
	};
public:
	OlaMATParser(tResourcePools* ps);

	virtual bool parseMATFromData(const char* data,int len,tMatFileInfo* outMatInfo);

	virtual bool fillMAT(tMatFileInfo* matInfo,OlaMaterial* outMat,MATFILL_MODE mode = CACHED_FILLMAT);

protected:
	tResourcePools* mPools;
};

#endif