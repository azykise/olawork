
#include "ola_tga.h"

#include "ola_resourcemng.h"
#include "ola_assetmng.h"

#include "ola_skeleton.h"
#include "ola_action_res.h"
#include "ola_animation.h"
#include "ola_device.h"

#include "ola_shaderpool.h"
#include "ola_texturepool.h"
#include "ola_materialpool.h"
#include "ola_meshpool.h"

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
const char TYPE_TRUE[] = "true";

#include "xml/tinyxml.h"
const char XML_ELEMENT_NAME[] = "name";
const char XML_ELEMENT_TYPE[] = "type";
const char XML_ELEMENT_VALUE[]= "value";

const char RESOURCE_FILETYPE_MATERIAL[] = "mat";

static OlaResourceMng* gResourceMngInstance = 0;

OlaResourceMng* OlaResourceMng::instance()
{
	return gResourceMngInstance;
}

OlaResourceMng::OlaResourceMng(OlaRenderDevice* device):
mLoader(0),
mPools(0),
mDevice(device)
{
	mLoader = new OlaAssetLoader();
	gResourceMngInstance = this;

	mPools = new tResourcePools();
	mPools->ShaderPool = new OlaShaderPool();
	mPools->TexturePool = new OlaTexturePool();
	mPools->MaterialPool = new OlaMaterialPool();
	mPools->MeshPool = new OlaMeshPool();	
}

OlaResourceMng::~OlaResourceMng()
{
	clear();

	if(mLoader)
	{
		delete mLoader;
		mLoader = 0;
	}

	if (mPools)
	{
		delete mPools->ShaderPool;
		delete mPools->TexturePool;
		delete mPools->MaterialPool;
		delete mPools->MeshPool;	
		delete mPools;
		mPools = 0;
	}

	gResourceMngInstance = 0;
}

void OlaResourceMng::clear()
{
	MeshTable::iterator i = mMeshs.begin();
	while( i != mMeshs.end() )
	{
		delete i->second;
		i++;
	}
	mMeshs.clear();	

	MaterialTable::iterator mr_i = mResourceMats.begin();
	while( mr_i != mResourceMats.end() )
	{
		delete mr_i->second;
		mr_i++;
	}
	mResourceMats.clear();
	MaterialTable::iterator ms_i = mSystemMats.begin();
	while( ms_i != mSystemMats.end() )
	{
		delete ms_i->second;
		ms_i++;
	}
	mSystemMats.clear();

	ShaderTable::iterator sr_i = mResourceShaders.begin();
	while( sr_i != mResourceShaders.end() )
	{
		delete sr_i->second;
		sr_i++;
	}
	mResourceShaders.clear();
	ShaderTable::iterator ss_i = mSystemShaders.begin();
	while( ss_i != mSystemShaders.end() )
	{
		delete ss_i->second;
		ss_i++;
	}
	mSystemShaders.clear();

	TextureTable::iterator texture_i = mTextures.begin();
	while ( texture_i != mTextures.end())
	{
		delete texture_i->second;
		texture_i++;
	}
	mTextures.clear();

	SkeletonTable::iterator skel_i = mSkeletons.begin();
	while ( skel_i != mSkeletons.end())
	{
		delete skel_i->second;
		skel_i++;
	}
	mSkeletons.clear();

	ActionResourceTable::iterator action_i = mActionResources.begin();
	while ( action_i != mActionResources.end())
	{
		delete action_i->second;
		action_i++;
	}
	mActionResources.clear();
}

OlaMesh* OlaResourceMng::getMesh(const char* _filename)
{
	olastring filename(_filename);
	MeshTable::iterator i = mMeshs.find(filename);
	if( i!= mMeshs.end())
	{
		return i->second;
	}
	return 0;
}

const char MATERIAL_DEFAULT[] = "default";
const char MATERIAL_HEAD[] = "ola_material";
const char MATERIAL_VERSION[] = "version";
const char MATERIAL_INSTANCE[] = "instance";
const char MATERIAL_TECHNIQUE[] = "technique";
const char MATERIAL_TECHSOURCEM[] = "source_m";
const char MATERIAL_TECHSOURCES[] = "source_s";
const char MATERIAL_TECH_SYS[]	= "sys";
const char MATERIAL_METANAME[]	  = "meta_name";
const char MATERIAL_TECHSOURCEV[] = "source_v";
const char MATERIAL_TECHSOURCEF[] = "source_f";
const char MATERIAL_VERTEX_ATTAIBUTE[] = "attrib";
const char MATERIAL_SHADER_VAR[] = "var";

void ParseMaterialVariablesFromXML(TiXmlElement* instance ,OlaMaterial* material ,OlaResourceMng* res_mng)
{
	TiXmlElement* var = instance->FirstChildElement(MATERIAL_SHADER_VAR);
	while (var)
	{
		const char* var_name = var->Attribute(XML_ELEMENT_NAME);
		const char* var_type = var->Attribute(XML_ELEMENT_TYPE);
		const char* var_value= var->Attribute(XML_ELEMENT_VALUE);
					
		if(strcmp(var_type,TYPE_TEXTURE) == 0)
		{				
			OlaTexture* diffuse_map = res_mng->getTexture(var_value);
			material->setParament(var_name,OlaMaterialParam::VALUE_TYPE_TEXTURE,diffuse_map);
		}
		else if(strcmp(var_type,TYPE_BOOL) == 0)
		{
			material->setParament(var_name,OlaMaterialParam::VALUE_TYPE_BOOL,var_value);
		}
		else if(strcmp(var_type,TYPE_VEC4) == 0)
		{
			material->setParament(var_name,OlaMaterialParam::VALUE_TYPE_VEC4,var_value);
		}

		var = var->NextSiblingElement(MATERIAL_SHADER_VAR);
	}
}

bool OlaResourceMng::reloadMaterial(const char* _filename,const char* mat_instancename)
{
	olastring findname(mat_instancename);

	OlaMaterial* material = 0;

	MaterialTable::iterator ri = mResourceMats.find(findname);
	if (ri != mResourceMats.end())
	{
		material = ri->second;
	}

	MaterialTable::iterator si = mSystemMats.find(findname);
	if (si != mSystemMats.end())
	{
		material = si->second;
	}

	if (!material)
	{
		return false;
	}

	OlaAsset* asset_mat = mLoader->load(_filename);
	assert( asset_mat && "material asset load error !" );

	TiXmlDocument xml;
	xml.Parse(asset_mat->data);
	assert( !xml.Error() && "material xml error !" );

	bool confirm = false;
	TiXmlElement* root = xml.FirstChildElement();
	const char* root_name = root->Value();
	assert( root && strcmp(root_name,MATERIAL_HEAD) == 0 && "material xml root error !");

	TiXmlElement* instance = root->FirstChildElement(MATERIAL_INSTANCE); 
	while(instance != 0)
	{
		const char* instance_name = instance->Attribute(XML_ELEMENT_NAME);

		if(strcmp(material->name().c_str(),instance_name) == 0)
		{			
			TiXmlElement* technique = instance->FirstChildElement(MATERIAL_TECHNIQUE);
			const char* tech_type = technique->Attribute(XML_ELEMENT_TYPE);							

			if (strcmp(tech_type,TYPE_SHADER) == 0)
			{
				const char* tech_source = technique->Attribute(MATERIAL_TECHSOURCES);
				const char* trunk_type = technique->Attribute(MATERIAL_TECH_SYS);

				RESOURCE_TRUNK_TYPE::TRUNK_TYPE t = trunk_type && !strcmp(trunk_type,TYPE_TRUE) ? RESOURCE_TRUNK_TYPE::SYSTEM_TRUNK : RESOURCE_TRUNK_TYPE::RESOURCE_TRUNK;

				olastring shader_name(tech_source);

				OlaShader* shader = 0;
				ShaderTable* pShaderTable = trunk_type ? &mSystemShaders : &mResourceShaders;
				ShaderTable& shader_table = *pShaderTable;

				ShaderTable::iterator shader_i = shader_table.find(shader_name);
				if (shader_i == shader_table.end())
				{
					shader = new OlaShader();
					OlaShaderFX* fx = GetRenderDevice()->spawnShaderFX();
					fx->load(shader_name.c_str());
					shader->reset(fx);
					shader_table[shader_name] = shader;
				}
				else
				{
					shader = shader_i->second;
					OlaShaderFX* fx = GetRenderDevice()->spawnShaderFX();
					fx->load(shader_name.c_str());
					shader->reset(fx);
				}

				material->setShader(shader);	
			}
			else if (strcmp(tech_type,TYPE_META) == 0)
			{
				const char* meta_source = technique->Attribute(MATERIAL_TECHSOURCEM);
				const char* meta_name = technique->Attribute(MATERIAL_METANAME);
				reloadMaterial(meta_source,meta_name);
			}		
		
			ParseMaterialVariablesFromXML(instance,material,this);

			return true;
		}

		instance = instance->NextSiblingElement(MATERIAL_INSTANCE);
	}

	return false;;
}

OlaMaterial* OlaResourceMng::getMaterial(const char* _filename,const char* mat_instancename)
{
	olastring findname(mat_instancename);

	MaterialTable::iterator si = mSystemMats.find(findname);
	if( si!= mSystemMats.end())
		return si->second;

	MaterialTable::iterator ri = mResourceMats.find(findname);
	if( ri!= mResourceMats.end())
		return ri->second;

	TiXmlDocument xml;

	OlaAsset* asset_mat = mLoader->load(_filename);
	assert( asset_mat && "material asset load error !" );
	
	xml.Parse(asset_mat->data);
	assert( !xml.Error() && "material xml error !" );

	bool confirm = false;
	TiXmlElement* root = xml.FirstChildElement();
	const char* root_name = root->Value();
	assert( root && strcmp(root_name,MATERIAL_HEAD) == 0 && "material xml root error !");

	OlaMaterial* return_mat = 0;

	TiXmlElement* instance = root->FirstChildElement(MATERIAL_INSTANCE); 
	while(instance != 0)
	{
		OlaMaterial* material = new OlaMaterial(_filename);

		const char* instance_name = instance->Attribute(XML_ELEMENT_NAME);
		olastring mat_name(instance_name);		
		material->setName(mat_name);

		TiXmlElement* technique = instance->FirstChildElement(MATERIAL_TECHNIQUE);
		const char* tech_type = technique->Attribute(XML_ELEMENT_TYPE);							
		const char* trunk_type = technique->Attribute(MATERIAL_TECH_SYS);
		RESOURCE_TRUNK_TYPE::TRUNK_TYPE t = trunk_type && !strcmp(trunk_type,TYPE_TRUE) ? RESOURCE_TRUNK_TYPE::SYSTEM_TRUNK : RESOURCE_TRUNK_TYPE::RESOURCE_TRUNK;

		if (strcmp(tech_type,TYPE_SHADER) == 0)
		{
			const char* tech_source = technique->Attribute(MATERIAL_TECHSOURCES);			
			olastring shader_name(tech_source);

			OlaShader* shader = 0;
			ShaderTable* pShaderTable = trunk_type && !strcmp(trunk_type,TYPE_TRUE)? &mSystemShaders : &mResourceShaders;
			ShaderTable& shader_table = *pShaderTable;

			ShaderTable::iterator shader_i = shader_table.find(shader_name);
			if (shader_i == shader_table.end())
			{
				shader = new OlaShader();
				OlaShaderFX* fx = GetRenderDevice()->spawnShaderFX();
				fx->load(shader_name.c_str());
				shader->reset(fx);
				shader_table[shader_name] = shader;
			}
			else
			{
				shader = shader_i->second;
			}

			material->setShader(shader);	
		}
		else if (strcmp(tech_type,TYPE_META) == 0)
		{
			const char* meta_source = technique->Attribute(MATERIAL_TECHSOURCEM);
			const char* meta_name = technique->Attribute(MATERIAL_METANAME);
			meta_name = meta_name == 0 ? MATERIAL_DEFAULT : meta_name;
			OlaMaterial* meta_mat = getMaterial(meta_source,meta_name);
		
			material->setFromMeta(meta_mat);
		}
		else
		{
			delete material;
			continue;
		}		
	
		ParseMaterialVariablesFromXML(instance,material,this);
	
		MaterialTable* pmatTable = trunk_type && !strcmp(trunk_type,TYPE_TRUE)? &mSystemMats : &mResourceMats;
		MaterialTable& mat_table = *pmatTable;
		mat_table[mat_name] = material;

		if(strcmp(mat_instancename,instance_name) == 0 && material)
		{
			return_mat = material;
		}

		instance = instance->NextSiblingElement(MATERIAL_INSTANCE);
	}

	return return_mat;
}

OlaTexture* OlaResourceMng::getTexture(const char* filename)
{
	olastring texture_name(filename);
	TextureTable::iterator i = mTextures.find(texture_name);
	if(i != mTextures.end())
		return i->second;

	OlaTexture* texture = mDevice->spawnTexture();

	OlaAsset* asset = mLoader->load(filename);

	if (!asset || !texture->readTga(asset))
	{
		lg("load asset: %s failed, fill with test pixel",filename);
		texture->filltest();
	}
	else
		lg("load testure %s done \n",filename);

	mTextures[texture_name] = texture;

	return texture;
}

OlaMaterial* OlaResourceMng::getPrimitiveMaterial()
{
	return getMaterial("data/prim_line.mat","prim_line");
}

OlaMaterial* OlaResourceMng::getShadowcastMaterial()
{
	return getMaterial("data/simple_shadowcast.mat","simple_shadowcast");
}

#include "ola_skeleton.h"
OlaSkeleton* OlaResourceMng::getSkeleton( const char* name,const char* filename )
{
	olastring skeleton_name(name);
	
	SkeletonTable::iterator i = mSkeletons.find(skeleton_name);
	if(i != mSkeletons.end())
	{
		return i->second;
	}

	OlaSkeleton* p = new OlaSkeleton("girl_skeleton");
	OlaAsset* asset = mLoader->load(filename);
	assert( asset && "load skeleton  assert error!");
	assert( p->load(asset) && "load girl skeleton error!");

	mSkeletons[skeleton_name] = p;

	return p;
}

OlaActionResource* OlaResourceMng::getActionResource(const char* filename)
{
	olastring action_name(filename);

	ActionResourceTable::iterator i = mActionResources.find(action_name);
	if(i != mActionResources.end())
	{
		return i->second;
	}

	OlaAsset* asset = OlaAssetLoader::instance()->load(filename);
	assert( asset && "Load Action Resource Asset Error !");

	OlaActionResource* p = new OlaActionResource(filename);
	assert( p->load(asset) && "Load Action Resource Error!");

	mActionResources[action_name] = p;
	return p;
}

void OlaResourceMng::clearResourceCache( const char* res_type )
{
	for (MeshTable::iterator i = mMeshs.begin() ; i != mMeshs.end();)
	{
		if (i->second->refCount() == 0)
		{
			delete i->second;
			mMeshs.erase(i++);
		}
		else
			++i;
	}

	for (MaterialTable::iterator i = mResourceMats.begin() ; i != mResourceMats.end();)
	{
		if (i->second->refCount() == 0)
		{
			delete i->second;
			mResourceMats.erase(i++);
		}
		else
			++i;
	}

	for (ShaderTable::iterator i = mResourceShaders.begin() ; i != mResourceShaders.end();)
	{
		if (i->second->refCount() == 0)
		{
			delete i->second;
			mResourceShaders.erase(i++);
		}
		else
			++i;
	}		
}

int OlaResourceMng::getResourceNum( const char* res_type )
{
	int num = 0 ;

	num += mResourceShaders.size();
	num += mResourceMats.size();
	num += mMeshs.size();

	return num;		
}

olastring OlaResourceMng::FilePathToAssetPath( const olastring& _filepath )
{
	olastring filepath = _filepath;
	filepath = olastring::toLower(filepath.accessData());
	filepath.replace("\\","/");
	
	olastring assetpath = "";

	int asset_idx = olastring::findText(filepath.c_str(),"assets/");
	if (asset_idx != -1)
	{
		assetpath = filepath.mid(asset_idx,filepath.length() - asset_idx);
	}

	return assetpath;
}
