#ifndef _OLA_MESH_POOL_H__
#define _OLA_MESH_POOL_H__

#include "ola_util.h"

class OlaMesh;
class OlaMeshPool
{
public:
	typedef OlaMap<olastring,OlaMesh*> MeshPool;
public:
	OlaMeshPool();

	virtual ~OlaMeshPool();

	void enPool(const char* aseassetpath,OlaMesh* mesh);
	void dePool(const char* aseassetpath);

	OlaMesh* seek(const char* aseassetpath);	

protected:
	MeshPool mMeshPool;
};

#endif