#ifndef _OLA_MATERIAL_H__
#define _OLA_MATERIAL_H__

#include <vector>

#include "ola_string.h"
#include "ola_resource.h"
#include "ola_texture.h"

#include "ola_assetmng.h"

#define MAX_MATERIAL_PARAM_STR_SIZE 128

class OlaShaderFX;

class OlaMaterialParam
{
public:
	enum PARAM_TYPE
	{
		PARAM_UNKNOWN,
		PARAM_VERTATTRIB,
		PARAM_SHADERVAR,
		PARAM_MATSYMBOL,
		PARAM_TEXLTYPE,
		PARAM_VALUETYPE,
	};

	enum SHADER_VARS
	{
		POSITION = 0,
		NORMAL,
		UV0,
		TANGENT,
		BINORMAL,

		SHADER_ATTRIB_END, //以上都是顶点属性

		MVP,
		MV,
		MODEL,
		MV_INVERSE,

		TEXTURE_PROJMAT,

		COLOR0,

		EYE_POS,
		EYE_DST,

		LIGHT_POS0,
		LIGHT_DIR0,
		LIGHT_COL0,

		SAMPLE_DIFFUSE,
		SAMPLE_NORMAL,
		SAMPLE_SPECULAR,
		SAMPLE_SHADOWMAP,		

		SHADER_VARS_END,//以上都是shader变量,不会向材质编辑暴露,
	};

	enum MATERIAL_SYMBOLS
	{
		TRANSPARENCY = 0,
		DIFFUSE0,
		NORMAL0,
		SPECULAR0,
		NO_CULLFACE,
		CAST_SHADOW,
		RECEIVE_SHADOW,
		ALPHA_TEST,
		NO_DEPTH,
		USER_COLOR0,

		MATERIAL_SYMBOLS_END,//以上都是渲染方式标记,向材质编辑暴露
	};

	enum VALUE_TYPE
	{
		VALUE_TYPE_UNKNOWN,
		VALUE_TYPE_INT,
		VALUE_TYPE_FLOAT,
		VALUE_TYPE_VEC4,
		VALUE_TYPE_BOOL,
		VALUE_TYPE_TEXTURE,
	};

	struct PARAM_DESC//mat文件输入的param type
	{
		PARAM_DESC(const char* _name,PARAM_TYPE t,int _index):name(_name),type(t),index(_index){}
		PARAM_TYPE type;
		int index;
		const char* name;
	};

	struct PARAM_VALUE //mat文件输入的param string值
	{
		PARAM_VALUE():type(VALUE_TYPE_UNKNOWN),data(0){}
		PARAM_VALUE(VALUE_TYPE _type,void* _data);
		PARAM_VALUE(VALUE_TYPE _type,const char* value_str);
		~PARAM_VALUE();
		VALUE_TYPE type;
		void* data;
	private:
		PARAM_VALUE(PARAM_VALUE&);
	};
};

class OlaShader : public IRefCounter
{
public:
	OlaShader();
	virtual ~OlaShader();

	void reset(OlaShaderFX* fx);	

	inline olastring& name(){return mName;}
	inline void* program();
	inline int handle(OlaMaterialParam::SHADER_VARS type){return type == OlaMaterialParam::SHADER_VARS_END ? -1 : mSystemHandles[type];}
	inline void setHandle(OlaMaterialParam::SHADER_VARS type,int value){ if(type != OlaMaterialParam::SHADER_ATTRIB_END){mSystemHandles[type] = value;}}
	inline OlaShaderFX* fx(){return mFX;}
protected:
	olastring mName;
	OlaShaderFX* mFX;

	int				mSystemHandles[OlaMaterialParam::SHADER_VARS_END];
	OlaArray<int>	mCustomHandles;
};

class OlaMaterial : public IRefCounter
{
public:

	OlaMaterial(const char* _filename);

	~OlaMaterial();	

	void setFromMeta(OlaMaterial* meta);

	void setShader(OlaShader* shader);
	OlaShader* shader(){return mShader;}

	void setParament(const char* name,OlaMaterialParam::VALUE_TYPE _type,const char* value,bool force=true);
	void setParament(const char* name,OlaMaterialParam::VALUE_TYPE _type,void* value,bool force=true);

	OlaMaterialParam::PARAM_VALUE* parament(const char* name);
	OlaMaterialParam::PARAM_VALUE* parament(OlaMaterialParam::MATERIAL_SYMBOLS type) {return mParaments[type];}

	void setName(olastring& str){mInstancename = str;};
	olastring& name(){return mInstancename;}

	olastring& filename(){return mFilename;}

	template <class T>
	T paramentValue(OlaMaterialParam::MATERIAL_SYMBOLS type)
	{
		OlaMaterialParam::PARAM_VALUE* s_v = parament(type);
		return s_v != 0 ? (T)(s_v->data) : 0 ;
	}

protected:

	olastring mInstancename;
	olastring mFilename;

	OlaShader* mShader;

	OlaMaterialParam::PARAM_VALUE* mParaments[OlaMaterialParam::MATERIAL_SYMBOLS_END];

	OlaMaterial* mMetaMaterial;
};

#endif
