#ifndef _OLA_RESOURCE_FILEPARSER_
#define _OLA_RESOURCE_FILEPARSER_

#include "../ola_util.h"

class OlaMesh;
class OlaShader;
class OlaSubMesh;
class OlaMaterial;
class OlaMeshPool;
class OlaShaderPool;
class OlaTexturePool;
class OlaMaterialPool;

struct tTplPropertyInfo
{
	olastring Name;
	olastring Type;
	olastring Value;
	olastring Bind;
};

struct tTplRenderStateInfo
{
	olastring Name;
	olastring Value;
};

struct tTplFileInfo
{
	olastring TplFullname;
	olastring FxType;
	OlaArray<tTplPropertyInfo>		Propertys;
	OlaArray<tTplRenderStateInfo>	RenderStates;

	olastring VertCodes;
	olastring FragCodes;
};

struct tMatVarInfo
{
	olastring Name;
	olastring Type;
	olastring Value;
};

struct tMatFileInfo
{
	olastring MatFullname;
	olastring ShaderFullname;	
	OlaArray<tMatVarInfo> Paraments;
};

struct tDmlFileInfo
{
	olastring DMLAssetpath;
	olastring ASEAssetpath;
	OlaArray<tMatFileInfo> MeshMatsInfo;
};

struct tResourcePools
{
	OlaShaderPool* ShaderPool;
	OlaMeshPool* MeshPool;
	OlaTexturePool* TexturePool;
	OlaMaterialPool* MaterialPool;	
};

#endif