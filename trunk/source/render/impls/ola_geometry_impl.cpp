#include "ola_geometry_impl.h"
#include "ola_material_impl.h"

#include "../ola_render.h"
#include "../ola_model.h"
#include "../win/win_impls.h"

#include "../parser/ola_dml.h"
#include "../ola_resourcemng.h"
#include "../ola_assetmng.h"

OlaGeometryImpl::OlaGeometryImpl(OlaMeshRenderer* model,OlaRender* render):
mModel(model),
mRender(render)
{
	mModel->addRef();

	for (size_t i = 0 ; i < mModel->mesh()->submeshs().size() ; i++)
	{
		OlaMaterial* material = mModel->material(i);
		OlaMaterialImpl* impl = new OlaMaterialImpl(material,render);
		mSubMaterials.push_back(impl);
	}
}

OlaGeometryImpl::~OlaGeometryImpl()
{
	for (size_t i = 0 ; i < mSubMaterials.size() ; i++)
	{
		OlaMaterialImpl* material = mSubMaterials[i];
		delete material;
	}	
	mSubMaterials.clear();

	if (mModel)
	{
		mModel->delRef();
		mModel = 0;
	}

}

OlaMeshRenderer* OlaGeometryImpl::meshrender()
{
	return mModel;
}

ola::IMaterial* OlaGeometryImpl::submeshMaterial(int idx)
{
	 return mSubMaterials[idx];
}

void OlaGeometryImpl::reload()
{
	OlaResourceMng* res_mng = mRender->getResourceMng();

	OlaAsset* xml_asset = res_mng->getLoader()->load(mModel->filename(),false);

	tDmlFileInfo dmlInfo;
	dmlInfo.DMLAssetpath = mModel->filename();

	OlaDMLParser parser(res_mng->pools());
	parser.parseDMLInfoFromData(xml_asset->data,xml_asset->length,&dmlInfo);
	delete xml_asset;

	parser.fillDML(&dmlInfo,mModel);

	for (size_t i = 0 ; i < mModel->mesh()->submeshs().size() ; i++)
	{
		OlaMaterial* material = mModel->material(i);

		if (i < mSubMaterials.size())
		{
			OlaMaterialImpl* impl = mSubMaterials[i];
			impl->material(material);
		}
		else
		{
			OlaMaterialImpl* impl = new OlaMaterialImpl(material,mRender);
			mSubMaterials.push_back(impl);
		}
	}
}
