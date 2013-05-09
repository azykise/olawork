#ifndef _OLA_MATERIAL_POOL_H__
#define _OLA_MATERIAL_POOL_H__

#include "ola_util.h"

class OlaMaterial;
class OlaMaterialPool
{
public:
	typedef OlaMap<olastring,OlaMaterial*> MaterialPool;
public:
	OlaMaterialPool();

	virtual ~OlaMaterialPool();

	void enPool(const char* matassetpath,OlaMaterial* mat);
	void dePool(const char* matassetpath);

	OlaMaterial* seek(const char* matassetpath);	

protected:
	MaterialPool mMaterialPool;
};

#endif