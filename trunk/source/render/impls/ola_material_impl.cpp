#include "ola_material_impl.h"

#include "../ola_render.h"
#include "../ola_material.h"
#include "../win/win_impls.h"
#include "../parser/ola_mat.h"
#include "../ola_render.h"
#include "../ola_resourcemng.h"
#include "../ola_assetmng.h"

OlaMaterialImpl::OlaMaterialImpl( OlaMaterial* mat , OlaRender* render):
mMaterial(mat),
mRender(render)
{
	mMaterial->addRef();
}

OlaMaterialImpl::~OlaMaterialImpl()
{
	mMaterial->delRef();
}

const char* OlaMaterialImpl::name()
{
	return mMaterial->name().c_str();
}

const char* OlaMaterialImpl::filename()
{
	return mMaterial->filename().c_str();
}

void OlaMaterialImpl::reload()
{
	tResourcePools* pools = mRender->getResourceMng()->pools();
	OlaMATParser parser(pools);

	tMatFileInfo matinfo;
	matinfo.MatFullname = mMaterial->filename();

	OlaAsset* asset = new OlaAsset();
	LoadAssetFile(matinfo.MatFullname.c_str(),asset);

	parser.parseMATFromData(asset->data,asset->length,&matinfo);
	parser.fillMAT(&matinfo,mMaterial,OlaMATParser::RELOAD_FILLMAT);		
}

bool OlaMaterialImpl::deserialize( const char* data,int len )
{
	return false;
}
