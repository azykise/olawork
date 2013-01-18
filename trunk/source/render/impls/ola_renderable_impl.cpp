#include "ola_renderable_impl.h"

#include "../ola_render.h"
#include "../ola_mesh.h"

#include "ola_geometry_impl.h"
#include "ola_rendercore_impl.h"

OlaRenderableImpl::OlaRenderableImpl(OlaRenderCoreImpl* parent):
mGeometry(0),
mParent(parent),
mActive(true)
{
	mTransform.Identity();
}

OlaRenderableImpl::~OlaRenderableImpl()
{
	_clearRenderOps();

	if(mParent->renderables().contains(this))
		mParent->renderables().remove(this);
}

void OlaRenderableImpl::_clearRenderOps()
{
	for (size_t i = 0 ; i < mRenderOps.size() ; i++)
	{
		OlaRenderOp* op = mRenderOps.at(i);
		delete op;
	}
	mRenderOps.clear();
}

void OlaRenderableImpl::setActive( bool a )
{
	mActive = a;
}

bool OlaRenderableImpl::isActive()
{
	return mActive;
}

void OlaRenderableImpl::setTransform( float* mat4_4 )
{
	mTransform.Set(mat4_4);

	RenderOpList::iterator i = mRenderOps.begin();
	while(i != mRenderOps.end())
	{
		(*i)->worldtrans.Set(mat4_4);
		i++;
	}		
}

void OlaRenderableImpl::setGeometry( ola::IGeometry* geo )
{
	OlaGeometryImpl* geo_impl = static_cast<OlaGeometryImpl*>(geo);
	if (geo_impl == mGeometry)
		return;
	
	_clearRenderOps();

	OlaMesh* mesh = geo_impl->mesh();

	OlaMesh::SubMeshList& submeh_list = mesh->submeshs();
	OlaMesh::SubMeshList::iterator i = submeh_list.begin();
	while(i != submeh_list.end())
	{
		OlaSubMesh* submesh = *i;

		OlaRenderOp* op = new OlaRenderOp(submesh->vb(),submesh->ib(),0);

		op->worldtrans.Set(mTransform.ToFloatPtr());

		mRenderOps.push_back(op);
		i++;
	}	

	mGeometry = geo_impl;	
}

ola::IGeometry* OlaRenderableImpl::geometry()
{
	return mGeometry;
}
