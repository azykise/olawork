#include "ola_shaderparser.h"

const std::string KW_SHADER_NAME("shadername:");
const std::string KW_SHADER_PROP("propertys");
const std::string KW_SHADER_TAGS("tags");

OlaShaderScriptParser::OlaShaderScriptParser()
{
	OlaShaderScript s;
	s.mShaderName = "xxx";
}

OlaShaderScriptParser::~OlaShaderScriptParser()
{

}

OlaShaderScript* OlaShaderScriptParser::NewOlaShaderScriptFromText( const std::string& text,std::string& outError )
{
	outError = "Shader Parse Error";

	return 0;
}
