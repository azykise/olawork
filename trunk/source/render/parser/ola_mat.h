#ifndef _OLA_MAT_FILEPARSER_
#define _OLA_MAT_FILEPARSER_

#include "../ola_util.h"

class OlaShader;
class OlaMaterial;
class OlaShaderPool;

struct tMatFileInfo
{
	olastring MatFullname;
	olastring ShaderFullname;
};

struct tMatResult
{
	OlaMaterial* Material;
};

class OlaMATParser
{
public:
	OlaMATParser(OlaShaderPool* shaderpool);

	virtual bool parseMATFromData(const char* data,int len,tMatFileInfo* outMatInfo);

	virtual bool fillMAT(tMatFileInfo* matInfo,tMatResult* outMat);

protected:
	OlaShaderPool* mShaderPool;
};

#endif