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
		std::string mValue;
	};

	struct tRenderState
	{
		std::string mName;
		std::vector<std::string> mValues;
	};

	struct tTag
	{
		std::string mName;
		std::string mValue;
	};

	struct tFunc
	{
		std::string mName;
		std::string mFuncStr;
	};

public:
	OlaShaderScript();
	OlaShaderScript(const std::string& name);
	virtual ~OlaShaderScript();

	const std::string& name();

protected:
	std::string mShaderName;
	std::vector<tTag> mTags;
	std::vector<tProperty> mPropertys;
	std::vector<tRenderState> mRenderStates;
	std::vector<tFunc> mFuncs;
};

#endif