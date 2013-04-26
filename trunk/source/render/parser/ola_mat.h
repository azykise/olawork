#ifndef _OLA_MAT_FILEPARSER_
#define _OLA_MAT_FILEPARSER_

#include "ola_parser.h"

class OlaMATParser
{
public:
	OlaMATParser(OlaShaderPool* shaderpool,OlaTexturePool* texlpool);

	virtual bool parseMATFromData(const char* data,int len,tMatFileInfo* outMatInfo);

	virtual bool fillMAT(tMatFileInfo* matInfo,tMatResult* outMat);

protected:
	OlaShaderPool* mShaderPool;
	OlaTexturePool* mTexturePool;
};

#endif