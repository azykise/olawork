#include "ola_shaderparser.h"

#include "util.h"

const std::string KW_SHADER_NAME("shadername:");
const std::string KW_SHADER_PROP("propertys");
const std::string KW_SHADER_TAGS("tags");
const std::string KW_SHADER_RENDERSTATES("renderstates");

OlaShaderScriptParser::OlaShaderScriptParser()
{
	OlaShaderScript s;
	s.mShaderName = "xxx";
}

OlaShaderScriptParser::~OlaShaderScriptParser()
{

}

OlaShaderScript* OlaShaderScriptParser::NewOlaShaderScriptFromText( const std::string& _text,std::string& outError )
{
	outError = "Shader Parse Error";

	OlaShaderScript* script = new OlaShaderScript();
	std::string text = _text;

	std::string::iterator i = text.begin();

	while(i != text.end())
	{
		outError = ReadNextTrunk(script,text,i);
		if(outError.length() != 0)
		{
			delete script;
			script = 0;
			break;
		}
	}

	return script;
}

std::string OlaShaderScriptParser::ReadNextTrunk( OlaShaderScript* script,const std::string& shaderText,std::string::iterator& index )
{
	std::string outError = "";

	while(index != shaderText.end())
	{
		std::string line = ::NextLine(shaderText,index);

		if(line.length() == 0)
		{

		}
		else if(line.find(KW_SHADER_NAME) != std::string::npos)
		{
			outError = ReadShaderName(script,line);
			if(outError.length())
				break;
		}
		else if(line.find(KW_SHADER_PROP) != std::string::npos)
		{
			outError = ReadShaderProps(script,shaderText,index);
			if(outError.length())
				break;
		}
		else if(line.find(KW_SHADER_TAGS) != std::string::npos)
		{
			outError = ReadShaderTags(script,shaderText,index);
			if(outError.length())
				break;
		}
		else if(line.find(KW_SHADER_RENDERSTATES) != std::string::npos)
		{
			outError = ReadShaderRenderStates(script,shaderText,index);
			if(outError.length())
				break;
		}
		else if(IsFunctionDescLine(line))
		{
			outError = ReadFunctions(script,line,shaderText,index);
			if(outError.length())
				break;
		}
	}

	return outError;
}

std::string OlaShaderScriptParser::ReadShaderName( OlaShaderScript* script,const std::string& line )
{
	std::string outError;
	unsigned int namePos = line.find(':');
	std::string shaderName = line.substr(namePos + 1);
	script->mShaderName = ::RemoveComment(shaderName);
	return outError;
}

std::string OlaShaderScriptParser::ReadShaderProps( OlaShaderScript* script,const std::string& shaderText,std::string::iterator& index )
{
	std::string outError;

	std::string line = ::NextLine(shaderText,index);
	line = ::RemoveComment(line);
	std::string lc = ::Trim(line);
	if(lc != "{")
	{
		outError = "{ Error! : " + line;
	}

	while(index != shaderText.end() && outError.size() == 0)
	{ 
		line = ::NextLine(shaderText,index);
		line = ::RemoveComment(line);
		line = ::Trim(line);

		if(line == "}")
			break;
		
		std::vector<std::string> prop_strs = ::Split(line,';');
		if(prop_strs.size() != 4)
		{
			outError = "Property Parse Error: " + line;
			break;
		}

		OlaShaderScript::tProperty prop;
		
		prop.mName = ::Trim(prop_strs[0]);
		prop.mDesc = ::Trim(prop_strs[1]);
		prop.mType = ::Trim(prop_strs[2]);
		prop.mValue = ::Trim(prop_strs[3]);

		script->mPropertys.push_back(prop);
	}

	return outError;
}

std::string OlaShaderScriptParser::ReadShaderRenderStates( OlaShaderScript* script,const std::string& shaderText,std::string::iterator& index )
{
	std::string outError;

	std::string line = ::NextLine(shaderText,index);
	line = ::RemoveComment(line);
	std::string lc = ::Trim(line);
	if(lc != "{")
	{
		outError = "{ Error! : " + line;
	}

	while(index != shaderText.end() && outError.size() == 0)
	{
		line = ::NextLine(shaderText,index);
		line = ::RemoveComment(line);
		line = ::Trim(line);

		if(line == "}")
			break;
		
		std::vector<std::string> renderstate_strs = ::Split(line,':');
		if(renderstate_strs.size() != 2)
		{
			outError = "RenderState Parse Error: " + line;
			break;
		}
		
		OlaShaderScript::tRenderState rs;
		rs.mName = renderstate_strs[0];
		
		std::vector<std::string> renderstate_vals = ::Split(renderstate_strs[1],',');		
		for(unsigned int i = 0 ; i < renderstate_vals.size() ; i++)
		{
			rs.mValues.push_back(::Trim(renderstate_vals[i]));
		}

		script->mRenderStates.push_back(rs);
	}

	return outError;
}

std::string OlaShaderScriptParser::ReadShaderTags( OlaShaderScript* script,const std::string& shaderText,std::string::iterator& index )
{
	std::string outError;

	std::string line = ::NextLine(shaderText,index);
	line = ::RemoveComment(line);
	std::string lc = ::Trim(line);
	if(lc != "{")
	{
		outError = "{ Error! : " + line;
	}

	while(index != shaderText.end() && outError.size() == 0)
	{
		line = ::NextLine(shaderText,index);
		line = ::RemoveComment(line);
		line = ::Trim(line);

		if(line == "}")
			break;
		
		std::vector<std::string> tag_strs = ::Split(line,':');
		if(tag_strs.size() != 2)
		{
			outError = "Tag Parse Error: " + line;
			break;
		}

		OlaShaderScript::tTag tag;
		
		tag.mName = ::Trim(tag_strs[0]);
		tag.mValue = ::Trim(tag_strs[1]);

		script->mTags.push_back(tag);
	}

	return outError;
}

#include<cctype>
bool OlaShaderScriptParser::IsFunctionDescLine( const std::string& line )
{
	int index_space = line.find(' ');
	if(index_space == std::string::npos)
		return false;

	int index_lbracket = line.find('(');
	if(index_lbracket == std::string::npos)
		return false;

	int index_rbracket = line.find(')');
	if(index_rbracket == std::string::npos)
		return false;

	if(index_space > index_lbracket || index_space > index_rbracket || index_lbracket > index_rbracket)
		return false;
	
	int lstart = index_space + 1;
	int len = index_lbracket - lstart;
	std::string funcName = line.substr(lstart,len);
	if(funcName.length() == 0)
		return false;

	for(unsigned int i = 0 ; i < funcName.length() ; i++)
	{
		char c = std::tolower(funcName[i]);
		if(c < 'a' || c > 'z')
		{
			if(c < '0' || c > '9')
			{
				if(c != '_')
					return false;
			}
		}
	}

	if(funcName[0] >= '0' && funcName[0] <= '9')
		return false;

	return true;
}

std::string OlaShaderScriptParser::ReadFunctions( OlaShaderScript* script,const std::string& funcNameLine,const std::string& shaderText,std::string::iterator& index )
{
	std::string outError;

	std::string line = ::NextLine(shaderText,index);
	line = ::RemoveComment(line);
	std::string lc = ::Trim(line);
	if(lc != "{")
	{
		outError = "{ Error! : " + line;
	}

	OlaShaderScript::tFunc func;
	func.mName = funcNameLine.substr(funcNameLine.find_first_of(' ') + 1,funcNameLine.find_first_of("("));
	func.mFuncStr.append(funcNameLine + "\n");
	func.mFuncStr.append(line + "\n");

	while(index != shaderText.end() && outError.size() == 0)
	{
		line = ::NextLine(shaderText,index);
		std::string _line = ::RemoveComment(line);
		_line = ::Trim(_line);				

		func.mFuncStr.append(line + "\n");

		if(_line == "}")
			break;		
	}

	script->mFuncs.push_back(func);

	return outError;
}
