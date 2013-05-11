#ifndef _OLA_MATERIAL_POOL_H__
#define _OLA_MATERIAL_POOL_H__

#include "ola_util.h"
struct tResourcePools;

class OlaMaterial;
class OlaMaterialPool
{
public:
	typedef OlaMap<olastring,OlaMaterial*> MaterialPool;
public:
	OlaMaterialPool(tResourcePools* ps);

	virtual ~OlaMaterialPool();

	void enPool(const char* matassetpath,OlaMaterial* mat);
	void dePool(const char* matassetpath);

	OlaMaterial* seek(const char* matassetpath);	

protected:
	MaterialPool mMaterialPool;

	OlaMaterial* mDefaultMaterial;
};

#endif