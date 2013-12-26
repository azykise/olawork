#ifndef _OLA_SHADER_SCRIPT_H__
#define _OLA_SHADER_SCRIPT_H__

#include <vector>
#include <string>

class OlaShaderScriptParser;

class OlaShaderScript
{
	friend OlaShaderScriptParser;
public:
	struct tProperty
	{
		std::string mName;
		std::string mDesc;
		std::string mType;
		std::string mvalue;
	};

	struct tRenderState
	{
		std::string mName;
		std::vector<std::string> mValues;
	};

public:
	OlaShaderScript();
	OlaShaderScript(const std::string& name);
	virtual ~OlaShaderScript();

	const std::string& name();

protected:
	std::string mShaderName;
	std::vector<std::string> mTags;
	std::vector<tProperty> mPropertys;
	std::vector<tRenderState> mRenderStates;
};

#endif