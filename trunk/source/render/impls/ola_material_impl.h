#ifndef _OLA_INCLUDE_MATERIAL_IMPLS_H__
#define _OLA_INCLUDE_MATERIAL_IMPLS_H__

#include "../ola_util.h"
#include "../../interface/ola_engine.h"

class OlaMaterial;
class OlaMaterialReference;
class OlaResourceMng;
class OlaRender;

class OlaMaterialImpl : public ola::IMaterial
{
public:
	OlaMaterialImpl(OlaMaterial* mat,OlaRender* render);

	virtual~ OlaMaterialImpl();

	virtual const char* name();

	virtual const char* filename();

	virtual void reload();

	virtual bool deserialize(const char* data,int len);

protected:
	OlaMaterial* mMaterial;
	OlaRender* mRender;
};

#endif