#include "ola_geometry_impl.h"
#include "ola_material_impl.h"

#include "../model.h"
#include "../ola_resourcemng.h"

OlaGeometryImpl::OlaGeometryImpl(OlaResourceMng* res_mng):
mResourceMng(res_mng)
{
	//for (size_t i = 0 ; i < mModel->mesh()->submeshs().size() ; i++)
	//{
	//	OlaMaterial* material = mModel->mesh()->submeshs()[i]->material();
	//	OlaMaterialImpl* impl = new OlaMaterialImpl(material,mResourceMng);
	//	mSubMaterials.push_back(impl);
	//}
}

OlaGeometryImpl::~OlaGeometryImpl()
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

ola::IMaterial* OlaGeometryImpl::material( int idx )
{
	return mSubMaterials[idx];
}

int OlaGeometryImpl::submeshNum()
{
	return mSubMaterials.size();
}

void OlaGeometryImpl::setMaterial( int idx , ola::IMaterial* mat )
{
	if (idx < mSubMaterials.size())
	{
		OlaMaterialImpl* mat_impl = static_cast<OlaMaterialImpl*>(mat);
		//mSubMaterials[idx]->setMaterial(mat_impl->material());
	}
}

ola::tAABB* OlaGeometryImpl::aabb()
{
	throw std::exception("The method or operation is not implemented.");
}

void OlaGeometryImpl::setRootBone( ola::ITransform* rb)
{
	throw std::exception("The method or operation is not implemented.");
}

ola::ITransform* OlaGeometryImpl::rootbone()
{
	throw std::exception("The method or operation is not implemented.");
}

int OlaGeometryImpl::deserialize( const ola::byte* data,int len )
{
	return 0;
}