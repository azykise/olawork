#include "ola_geometry_impl.h"
#include "ola_material_impl.h"

#include "../ola_render.h"
#include "../ola_model.h"
#include "../win/win_impls.h"


OlaGeometryImpl::OlaGeometryImpl(OlaMeshRenderer* model,OlaRender* render):
mModel(model)
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
