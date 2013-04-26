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
	olastring DmlFullname;
	olastring ASEFullname;
	OlaArray<tMatFileInfo> MeshMatsInfo;
};

struct tMatResult
{
	OlaMaterial* Material;
};

struct tDmlResult
{
	OlaMesh* Mesh;
	OlaArray<OlaMaterial*> MeshMats;
};

struct tResourcePools
{
	OlaShaderPool* ShaderPool;
	OlaMeshPool* MeshPool;
	OlaTexturePool* TexturePool;
	OlaMaterialPool* MaterialPool;	
};

#endif