#include "ola_shaderscript.h"

OlaShaderScript::OlaShaderScript()
{

}

OlaShaderScript::OlaShaderScript( const std::string& name ):
mShaderName(name)
{

}

OlaShaderScript::~OlaShaderScript()
{

}

const std::string& OlaShaderScript::name()
{
	return mShaderName;
}
