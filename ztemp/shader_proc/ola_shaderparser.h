#ifndef _OLA_SHADER_SRCIPT_PARSER_H__
#define _OLA_SHADER_SRCIPT_PARSER_H__

#include "ola_shaderscript.h"

class OlaShaderScriptParser
{
public:
	OlaShaderScriptParser();
	virtual ~OlaShaderScriptParser();

	virtual OlaShaderScript* NewOlaShaderScriptFromText(const std::string& text,std::string& outError);

protected:

};

#endif