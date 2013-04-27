#include "ola_model.h"
#include "ola_mesh.h"
#include "ola_material.h"
#include "ola_render.h"

OlaMeshRenderer::OlaMeshRenderer():
mMesh(0),
mTranform(0)
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

	mTranform = 0;
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

OlaTransformObj* OlaMeshRenderer::transform()
{
	return mTranform;
}

void OlaMeshRenderer::transform( OlaTransformObj* transobj )
{
	mTranform = transobj;
}

