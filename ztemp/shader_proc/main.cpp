#include "preinclude.h"
#include "util.h"

#include "ola_shaderparser.h"

OlaShaderScriptParser gScriptParser;

int main(int argc,char* argv[])
{
	std::vector<std::string> lines;

	std::string shaderText = ::ReadAllText("./test0.shader");
		
	std::string outError;
	OlaShaderScript* s = gScriptParser.NewOlaShaderScriptFromText(shaderText,outError);

	lines.push_back(outError);

	for (unsigned int i = 0 ; i < lines.size() ; i++)
	{
		std::cout<<lines[i]<<std::endl;
	}

	delete s;

	std::system("pause");

	return 0;
}

#include "shader_proc.h"

#define DLLEXPORT __declspec(dllexport)

extern "C"
{
	DLLEXPORT void* OSS_Create(char* filename)
	{
		return 0;
	}

	DLLEXPORT void OSS_Release(OlaShaderScript* script)
	{

	}

	DLLEXPORT void OSS_GetName(OlaShaderScript* script,char* nameBuffer)
	{

	}

	DLLEXPORT int OSS_GetPropertyNum(OlaShaderScript* script)
	{
		return 0;
	}

	DLLEXPORT int OSS_GetPropertyNum(OlaShaderScript* script)
	{

	}

}

