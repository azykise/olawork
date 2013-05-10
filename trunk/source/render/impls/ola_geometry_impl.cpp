#include "ola_geometry_impl.h"

#include "../ola_render.h"
#include "../ola_model.h"
#include "../win/win_impls.h"


OlaGeometryImpl::OlaGeometryImpl(OlaMeshRenderer* model):
mModel(model)
{
	mModel->addRef();

	for (size_t i = 0 ; i < mModel->mesh()->submeshs().size() ; i++)
	{
		OlaMaterial* material = mModel->material(i);
		OlaMaterialImpl* impl = new OlaMaterialImpl(material);
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

OlaMesh* OlaGeometryImpl::mesh()
{
	return mModel->mesh();
}

OlaArray<OlaRenderOp*>& OlaGeometryImpl::renderOps()
{
	return mModel->updateRenderOps();
}

ola::IMaterial* OlaGeometryImpl::submeshMaterial(int idx)
{
	 return mSubMaterials[idx];
}
