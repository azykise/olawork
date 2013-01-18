#ifndef _OLA_INCLUDE_MATERIAL_IMPLS_H__
#define _OLA_INCLUDE_MATERIAL_IMPLS_H__

#include "../../interface/ola_irendercore.h"

#include "../ola_material_ref.h"

class OlaMaterial;
class OlaMaterialReference;
class OlaResourceMng;
class OlaMaterialImpl : public ola::IMaterial
{
public:
	OlaMaterialImpl(OlaMaterialReference* mat);

	virtual~ OlaMaterialImpl();

	virtual const char* name();

	virtual int deserialize(const ola::byte* data,int len);

	inline OlaMaterial* material(){return mMatRef ? 0 : mMatRef->material();}

protected:
	OlaMaterialReference* mMatRef;
};

#endif