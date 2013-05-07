#include "ola_geometry_impl.h"
//#include "ola_material_impl.h"

#include "../ola_model.h"

OlaMeshGeometryImpl::OlaMeshGeometryImpl():
mMesh(0)
{
	//for (size_t i = 0 ; i < mModel->mesh()->submeshs().size() ; i++)
	//{
	//	OlaMaterial* material = mModel->mesh()->submeshs()[i]->material();
	//	OlaMaterialImpl* impl = new OlaMaterialImpl(material,mResourceMng);
	//	mSubMaterials.push_back(impl);
	//}
}

OlaMeshGeometryImpl::~OlaMeshGeometryImpl()
{
	//for (size_t i = 0 ; i < mSubMaterials.size() ; i++)
	//{
	//	OlaMaterialImpl* material = mSubMaterials[i];
	//	delete material;
	//}	
	//mSubMaterials.clear();

	//if (mModel)
	//{
	//	delete mModel;
	//	mModel = 0;
	//}

}

OlaArray<OlaRenderOp*>& OlaMeshGeometryImpl::renderOps()
{
	return mMesh->updateRenderOps();
}
