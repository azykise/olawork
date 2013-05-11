#include "ola_model.h"
#include "ola_mesh.h"
#include "ola_material.h"
#include "ola_render.h"
#include "ola_transobj.h"

OlaMeshRenderer::OlaMeshRenderer(const char* dmlassetpath):
mMesh(0),
mDMLAssetpath(dmlassetpath)
{
	
}

OlaMeshRenderer::~OlaMeshRenderer()
{
	if (mMesh)
	{
		mMesh->delRef();
		mMesh = 0;
	}

	for (unsigned int i = 0 ; i < mMaterials.size() ; i++)
	{
		if (mMaterials[i] != 0)
		{
			mMaterials[i]->delRef();			
		}
	}
	mMaterials.clear();

	for (unsigned int i = 0 ; i < mRenderOps.size() ; i++)
	{
		if (mRenderOps[i] != 0)
		{
			delete mRenderOps[i];
		}
	}
	mRenderOps.clear();
}

OlaMesh* OlaMeshRenderer::mesh()
{
	return mMesh;
}

void OlaMeshRenderer::mesh( OlaMesh* mesh /*= 0*/ )
{
	if ( mesh == mMesh )
	{
		return;
	}

	if ( !mesh )
	{
		if ( mMesh )
		{
			mMesh->delRef();			
		}
		mMesh = 0;
	}
	else
	{
		if ( mMesh )
		{
			mMesh->delRef();
		}
		mesh->addRef();
		mMesh = mesh;		
	}

	if ( mMesh )
	{
		for (unsigned int i = 0 ; i < mMaterials.size() ; i++ )
		{
			if (mMaterials[i] != 0)
			{
				mMaterials[i]->delRef();
			}			
		}
		mMaterials.clear();
		mMaterials.resize(mMesh->submeshs().size());
		mMaterials.assign(mMesh->submeshs().size(),0);

		for (unsigned int i = 0 ; i < mRenderOps.size() ; i++)
		{
			if (mRenderOps[i] != 0)
			{
				delete mRenderOps[i];
				mRenderOps[i] = 0;
			}
		}

		mRenderOps.resize(mMesh->submeshs().size());
		
		for (unsigned int i = 0 ; i < mMesh->submeshs().size() ; i++)
		{
			OlaSubMesh* submesh = mMesh->submeshs()[i];
			OlaRenderOp* op = new OlaRenderOp(submesh);
			mRenderOps[i] = op;
		}
	}
}

OlaMaterial* OlaMeshRenderer::material( int index )
{
	if ( !mMesh || index < 0 || index >= mMaterials.size())
	{
		return 0;
	}
	return mMaterials[index];
}

void OlaMeshRenderer::material( int index,OlaMaterial* mat )
{
	if( !mMesh || index < 0 || index >= mMaterials.size() )
		return;

	if( mMaterials[index] != 0 )
	{
		mMaterials[index]->delRef();
	}

	mat->addRef();
	mMaterials[index] = mat;

	mRenderOps[index]->material(mat);
}

const char* OlaMeshRenderer::kernelID()
{
	return mDMLAssetpath.c_str();
}

OlaKernelObj::ENABLE_STATE OlaMeshRenderer::enabled()
{
	return ES_ENABLE_ALL;
}

void OlaMeshRenderer::updateInternal( float elasped , OlaTransformObj* transObj)
{
	for (unsigned int i = 0; i < mRenderOps.size() ; i++)
	{
		OlaRenderOp* op = mRenderOps[i];

		if (transObj)
		{
			const float* f44 = transObj->transform().ToFloatPtr();
			op->worldtrans.FromFloatsColumnMajor(f44);
		}
		else
		{
			op->worldtrans.Identity();
		}

	}
}

void OlaMeshRenderer::renderInternal( OlaRender* r )
{
	for (unsigned int i = 0 ; i < mRenderOps.size() ; i++)
	{
		OlaRenderOp* op = mRenderOps[i];			

		r->pushToRender(op);
	}
}

