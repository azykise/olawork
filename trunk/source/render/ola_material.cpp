#include "ola_material.h"
#include "math/ola_math.h"

#ifdef WIN32
#pragma warning(disable:4996)
#endif

#include "ola_shaderfx.h"

extern const char gGlobalShaderConstName[OlaMaterialParam::SHADER_VARS_END][MAX_MATERIAL_PARAM_STR_SIZE] = {
	"a_position",
	"a_normal",
	"a_uv0",
	"a_tangent",
	"a_binormal",

	"vertex_attribute_end",

	"u_mvp",
	"u_mv",
	"u_model",
	"u_model_invtrans",

	"u_texture_proj",

	"u_color0",

	"u_eyepos",
	"u_eyedir",

	"u_lightpos0",
	"u_lightdir0",
	"u_lightcol0",

	"s_diffuse",
	"s_normal",
	"s_specular",
	"s_shadowmap"
};

static const char gGlobalMaterialSymbol[OlaMaterialParam::MATERIAL_SYMBOLS_END][MAX_MATERIAL_PARAM_STR_SIZE] = {
	"transparency",
	"diffuse_map",
	"normal_map",
	"specular_map",
	"no_cullface",
	"cast_shadow",
	"receive_shadow",
	"alpha_test",
	"no_depth",
	"user_color0"
};

static OlaMaterialParam::PARAM_DESC _GetParamDesc(const char* var_name)
{
	OlaMaterialParam::PARAM_TYPE type = OlaMaterialParam::PARAM_UNKNOWN;
	int index = -1;

	if(type == OlaMaterialParam::PARAM_UNKNOWN)
	{
		for (int i = 0 ; i < OlaMaterialParam::SHADER_VARS_END ; i++)
		{
			if(strcmp(var_name,gGlobalShaderConstName[i]) == 0)
			{
				type = OlaMaterialParam::PARAM_SHADERVAR;
				index = i;
				break;
			}
		}
	}

	if(type == OlaMaterialParam::PARAM_UNKNOWN)
	{
		for (int i = 0 ; i < OlaMaterialParam::MATERIAL_SYMBOLS_END ; i++)
		{
			if(strcmp(var_name,gGlobalMaterialSymbol[i]) == 0)
			{
				type = OlaMaterialParam::PARAM_MATSYMBOL;
				index = i;
				break;
			}
		}
	}

	OlaMaterialParam::PARAM_DESC desc(var_name,type,index);
	return desc;
}

unsigned int gBoolValue = 1;
OlaMaterialParam::PARAM_VALUE::PARAM_VALUE(VALUE_TYPE _type,const char* value_str):
type(_type),
data(0)
{
	switch(type)
	{
		case VALUE_TYPE_INT:
			data = new int(0);
			sscanf(value_str,"%d",data);
			break;
		case VALUE_TYPE_FLOAT:
			data = new float(0.0f);
			sscanf(value_str,"%f",data);
			break;
		case VALUE_TYPE_VEC4:
			data = new olaVec4(0,0,0,0);
			sscanf(value_str,"(%f,%f,%f,%f)",&(((olaVec4*)data)->x),&(((olaVec4*)data)->y),&(((olaVec4*)data)->z),&(((olaVec4*)data)->w));
			break;
		case VALUE_TYPE_BOOL:
			if(strcmp(value_str,"true") == 0 )
				memcpy(&data,&gBoolValue,sizeof(void*));
			else
				data = 0;
			break;
		case VALUE_TYPE_TEXTURE:
			break;
	}
}

OlaMaterialParam::PARAM_VALUE::PARAM_VALUE(VALUE_TYPE _type,void* _data):
type(_type),
data(0)
{
	switch(type)
	{
		case VALUE_TYPE_INT:
			data = new int(0);
			memcpy(data,_data,sizeof(int));
			break;
		case VALUE_TYPE_FLOAT:
			data = new float(0.0f);
			memcpy(data,_data,sizeof(float));
			break;
		case VALUE_TYPE_VEC4:
			data = new olaVec4(0,0,0,0);
			memcpy(data,_data,sizeof(olaVec4));
			break;
		case VALUE_TYPE_BOOL:
			data = _data;
			break;
		case VALUE_TYPE_TEXTURE:
			data = _data;
			break;
	}	
}

OlaMaterialParam::PARAM_VALUE::~PARAM_VALUE()
{
	if(data)
	{
		switch(type)
		{
			case VALUE_TYPE_INT:
			case VALUE_TYPE_FLOAT:
			case VALUE_TYPE_VEC4:
				delete data;
				data = 0;
				break;
			case VALUE_TYPE_BOOL:
			case VALUE_TYPE_TEXTURE:
				data = 0;
				break;
		}
	}
}

static void bindShaderVar(OlaShader* shader,OlaMaterialParam::SHADER_VARS type)
{
	int handle = shader->handle(type);
	int program = *static_cast<int*>(shader->program());
	if(handle == -1 && type != OlaMaterialParam::SHADER_VARS_END)
	{
		handle = shader->fx()->requestHandle(type);
		shader->setHandle(type,handle);
	}	
}

OlaShader::OlaShader():
mName("default"),
mFX(0)
{
	memset(mHandles,-1,OlaMaterialParam::SHADER_VARS_END * sizeof(int));
}

void OlaShader::reset(OlaShaderFX* fx)
{
	if(mFX)
	{
		delete mFX;
		mFX = 0;
	}

	mFX = fx;

	mName = mFX->filename();

	memset(mHandles,-1,OlaMaterialParam::SHADER_VARS_END * sizeof(int));

	for(int i = OlaMaterialParam::POSITION ; i < OlaMaterialParam::SHADER_VARS_END ; i++)
	{
		OlaMaterialParam::SHADER_VARS t = (OlaMaterialParam::SHADER_VARS)i;
		bindShaderVar(this,t);
	}
}

OlaShader::~OlaShader()
{
	if(mFX)
	{
		delete mFX;
		mFX = 0;
	}
	lg("~OlaShader()");
}

void* OlaShader::program()
{
	return mFX->program();
}
OlaMaterial::OlaMaterial(const char* _filename):
mShader(0),
mMetaMaterial(0),
mFilename(_filename)
{
	memset(mSymbols,0,OlaMaterialParam::MATERIAL_SYMBOLS_END * sizeof(OlaMaterialParam::PARAM_VALUE*));
}

OlaMaterial::~OlaMaterial()
{
	int var = OlaMaterialParam::TRANSPARENCY;
	while (var != OlaMaterialParam::MATERIAL_SYMBOLS_END)
	{
		OlaMaterialParam::PARAM_VALUE* value = mSymbols[var];
		if(value != 0)
		{
			delete value;
			mSymbols[var] = 0;
		}
		var += 1;
	}

	if(mMetaMaterial)
	{
		mMetaMaterial->delRef();
		mMetaMaterial = 0;
	}

	if (mShader)
	{
		mShader->delRef();
		mShader = 0;
	}
}

void OlaMaterial::setShader(OlaShader* shader)
{
	if(mShader)
		mShader->delRef();

	mShader = shader;
	mShader->addRef();
}

void OlaMaterial::setFromMeta(OlaMaterial* meta)
{
	if(mMetaMaterial)
		mMetaMaterial->delRef();

	mMetaMaterial = meta;
	mMetaMaterial->addRef();

	setShader(meta->shader());

	int var = OlaMaterialParam::TRANSPARENCY;
	while (var != OlaMaterialParam::MATERIAL_SYMBOLS_END)
	{
		OlaMaterialParam::PARAM_VALUE* met_value = meta->mSymbols[var];
		if(met_value != 0)
		{
			OlaMaterialParam::PARAM_VALUE* value = new OlaMaterialParam::PARAM_VALUE(met_value->type,met_value->data);
			if(mSymbols[var] == 0)
				mSymbols[var] = value;
			else
			{
				delete mSymbols[var];
				mSymbols[var] = value;
			}
		}
		var += 1;
	}
}

void OlaMaterial::setSymbol(const char* name,OlaMaterialParam::VALUE_TYPE _type,const char* value,bool force)
{
	OlaMaterialParam::PARAM_DESC desc = _GetParamDesc(name);
	if(desc.type == OlaMaterialParam::PARAM_MATSYMBOL)
	{
		OlaMaterialParam::PARAM_VALUE* pvalue = new OlaMaterialParam::PARAM_VALUE(_type,value);
		if(mSymbols[desc.index] == 0)
			mSymbols[desc.index] = pvalue;
		else
		{
			delete mSymbols[desc.index];
			mSymbols[desc.index] = pvalue;
		}
	}
}

void OlaMaterial::setSymbol(const char* name,OlaMaterialParam::VALUE_TYPE _type,void* value,bool force)
{
	OlaMaterialParam::PARAM_DESC desc = _GetParamDesc(name);
	if(desc.type == OlaMaterialParam::PARAM_MATSYMBOL)
	{
		OlaMaterialParam::PARAM_VALUE* pvalue = new OlaMaterialParam::PARAM_VALUE(_type,value);
		if(mSymbols[desc.index] == 0)
			mSymbols[desc.index] = pvalue;
		else
		{
			delete mSymbols[desc.index];
			mSymbols[desc.index] = pvalue;
		}
	}
}

OlaMaterialParam::PARAM_VALUE* OlaMaterial::symbol(const char* name)
{
	OlaMaterialParam::PARAM_DESC desc = _GetParamDesc(name);
	if(desc.type == OlaMaterialParam::PARAM_MATSYMBOL)
	{
		return mSymbols[desc.index];
	}
	else
	{
		return 0;
	}
}
