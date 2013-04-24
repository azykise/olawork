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

	void enPool(OlaMaterial* mat);
	void dePool(OlaMaterial* mat);
	void dePool(const char* matfilename);

	OlaMaterial* seek(const char* matfilename);	

protected:
	MaterialPool mMaterialPool;
};

#endif