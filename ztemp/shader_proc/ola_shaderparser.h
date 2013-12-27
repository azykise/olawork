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
	std::string ReadFunctions(OlaShaderScript* script,const std::string& funcName,const std::string& shaderText,std::string::iterator& index);
	std::string ReadShaderTags( OlaShaderScript* script,const std::string& shaderText,std::string::iterator& index );
	std::string ReadShaderRenderStates( OlaShaderScript* script,const std::string& shaderText,std::string::iterator& index );
	std::string ReadShaderProps( OlaShaderScript* script,const std::string& shaderText,std::string::iterator& index);
	std::string ReadShaderName(OlaShaderScript* scrpit,const std::string& line);
	std::string ReadNextTrunk(OlaShaderScript* script,const std::string& shaderText,std::string::iterator& index);

	bool IsFunctionDescLine(const std::string& line);
};

#endif