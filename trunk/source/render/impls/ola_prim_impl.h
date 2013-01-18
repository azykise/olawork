#ifndef _OLA_INCLUDE_PRIMITIVE_IMPLS_H__
#define _OLA_INCLUDE_PRIMITIVE_IMPLS_H__

#include "../ola_stl.h"
#include "../../interface/ola_irendercore.h"

class OlaPrimitive;
class OlaPrimitiveImpl : public ola::IPrimitive
{
public:
	OlaPrimitiveImpl(OlaPrimitive* prim);

	virtual ~OlaPrimitiveImpl();

	virtual unsigned long long getID();

	virtual ola::PrimitiveType getType() {return ola::PMT_LINE;};

	virtual void setVertex(float* vec3s,int length);

	virtual void setColor(float r,float g,float b,float a);

	virtual void setIndex(int* idxs,int length);

	OlaPrimitive* mPrimitive;
};

#endif