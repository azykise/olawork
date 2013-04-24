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

	void enPool(OlaMesh* mat);
	void dePool(OlaMesh* mat);
	void dePool(const char* asefilename);

	OlaMesh* seek(const char* asefilename);	

protected:
	MeshPool mMeshPool;
};

#endif