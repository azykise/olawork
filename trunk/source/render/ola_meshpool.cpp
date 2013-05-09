#include "ola_meshpool.h"
#include "ola_mesh.h"

OlaMeshPool::OlaMeshPool()
{

}

OlaMeshPool::~OlaMeshPool()
{

}

void OlaMeshPool::enPool( const char* aseassetpath , OlaMesh* mesh )
{
	olastring _aseassetpath(aseassetpath);
	MeshPool::iterator i = mMeshPool.find(_aseassetpath);
	if (i == mMeshPool.end())
	{
		mesh->addRef();
		mMeshPool[_aseassetpath] = mesh;
	}
}

void OlaMeshPool::dePool( const char* aseassetpath )
{
	olastring _aseassetpath(aseassetpath);
	MeshPool::iterator i = mMeshPool.find(_aseassetpath);
	if (i != mMeshPool.end())
	{
		OlaMesh* mesh = i->second;
		mesh->delRef();
		if (mesh->refCount() == 0)
		{
			mMeshPool.erase(_aseassetpath);
			delete mesh;
		}
	}
}

OlaMesh* OlaMeshPool::seek( const char* aseassetpath )
{
	olastring _aseassetpath(aseassetpath);

	MeshPool::iterator i = mMeshPool.find(_aseassetpath);
	if (i != mMeshPool.end())
	{
		i->second->addRef();
		return i->second;
	}

	return 0;
}