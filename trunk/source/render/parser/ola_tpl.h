#ifndef _OLA_TPL_FILEPARSER_
#define _OLA_TPL_FILEPARSER_

#include "ola_parser.h"

class OlaShader;
class OlaTPLParser
{
public:
	OlaTPLParser(tResourcePools* ps);

	virtual bool parseTPLFromData(const char* data,int len,tTplFileInfo* outTplInfo);

	virtual bool fillTPL(tTplFileInfo* matInfo,OlaShader* outShader);

protected:
	tResourcePools* mPools;	
};

#endif