#include "ola_mat.h"

#include "../xml/ola_xml.h"

#include "../ola_device.h"
#include "../ola_assetmng.h"

#include "../ola_shaderfx.h"
#include "../ola_material.h"
#include "../ola_shaderpool.h"
#include "../ola_texturepool.h"

const char TYPE_STRING[] = "string";
const char TYPE_FLOAT[] = "float";
const char TYPE_INT[] = "int";
const char TYPE_VEC2[] = "vec2";
const char TYPE_VEC3[] = "vec3";
const char TYPE_VEC4[] = "vec4";
const char TYPE_MAT4[] = "mat4";
const char TYPE_QUAT[] = "quat";
const char TYPE_SHADER[] = "shader";
const char TYPE_META[] = "meta";
const char TYPE_TEXTURE[] = "texture";
const char TYPE_BOOL[]	= "bool";

OlaMATParser::OlaMATParser( tResourcePools* ps ):
mPools(ps)
{

}

bool OlaMATParser::parseMATFromData( const char* data,int len,tMatFileInfo* outMatInfo )
{
	OlaXmlDocument* xml = OlaXmlDocument::CreateXmlDocument();
	assert( xml->parse(data) && "mat parse error" );

	OlaXmlNode* mat_node = xml->fisrtChild();
	assert(mat_node && "no mat_node!");

	OlaXmlNode* ins_node = mat_node->fisrtChild();

	OlaXmlNode* tech_node = ins_node->selectFirst("technique");
	const char* tech_type = tech_node->attribute("type");
	const char* shader = tech_node->attribute("source_s");

	outMatInfo->ShaderFullname = shader;

	OlaXmlNode* var_node = ins_node->selectFirst("var");
	while(var_node != 0)
	{
		tMatVarInfo varinfo;
		varinfo.Name = var_node->attribute("name");
		varinfo.Type = var_node->attribute("type");
		varinfo.Value = var_node->attribute("value");

		outMatInfo->Paraments.push_back(varinfo);

		var_node = var_node->nextSibling("var");
	}

	return true;
}

bool OlaMATParser::fillMAT( tMatFileInfo* matInfo,OlaMaterial* outMat, MATFILL_MODE mode )
{
	bool loadfx = false;

	OlaShader* shader = 0;
	shader = mPools->ShaderPool->seek(matInfo->ShaderFullname.c_str());
	if(shader == 0)
	{
		shader = new OlaShader();
		mPools->ShaderPool->enPool(matInfo->ShaderFullname.c_str(),shader);
		loadfx = true;
	}

	switch(mode)
	{
	case CACHED_FILLMAT:
		break;
	case RELOAD_FILLMAT:
		loadfx = true;
		break;;
	}

	if (loadfx)
	{
		OlaShaderFX* fx = GetRenderDevice()->spawnShaderFX();
		fx->load(matInfo->ShaderFullname.c_str());
		shader->reset(fx);
	}

	OlaMaterial* material = outMat;
	material->setShader(shader);

	for (unsigned int i = 0 ; i < matInfo->Paraments.size() ; i++)
	{
		tMatVarInfo& var = matInfo->Paraments[i];

		if(strcmp(var.Type.c_str(),TYPE_TEXTURE) != 0)				
			continue;

		OlaTexture* texture = mPools->TexturePool->seek(var.Value.c_str());
		if(!texture)	
		{
			OlaAsset* asset = new OlaAsset();
			if(LoadAssetFile(var.Value.c_str(),asset))
			{
				texture = GetRenderDevice()->spawnTexture();

				texture->readTga(asset->data,asset->length);
			}
			else
				texture = mPools->TexturePool->deftexture();

			delete asset;

			mPools->TexturePool->enPool(var.Value.c_str(),texture);
		}

		//fixme: 纹理的引用计数怎么搞？ ---> done:在PARAM_VAR的构造和析构中处理引用计数
		material->setParament(var.Name.c_str(),OlaMaterialParam::VALUE_TYPE_TEXTURE,texture);
	}

	for (unsigned int i = 0 ; i < matInfo->Paraments.size() ; i++)
	{
		tMatVarInfo& var = matInfo->Paraments[i];

		if(strcmp(var.Type.c_str(),TYPE_BOOL) == 0)
		{
			material->setParament(var.Name.c_str(),OlaMaterialParam::VALUE_TYPE_BOOL,var.Value.c_str());
		}
		else if(strcmp(var.Type.c_str(),TYPE_VEC4) == 0)
		{
			material->setParament(var.Name.c_str(),OlaMaterialParam::VALUE_TYPE_VEC4,var.Value.c_str());
		}
	}

	return true;
}