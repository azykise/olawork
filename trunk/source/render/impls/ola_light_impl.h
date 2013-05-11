#ifndef _OLA_INCLUDE_LIGHT_IMPLS_H__
#define _OLA_INCLUDE_LIGHT_IMPLS_H__

#include "../ola_util.h"
#include "../../interface/ola_engine.h"

class OlaTransformImpl;
class OlaLight;

class OlaLightImpl : public ola::ILight
{
public:
	OlaLightImpl(OlaLight* l);

	virtual ~OlaLightImpl();

	virtual ola::aabb* aabb() {return 0;}

	virtual ola::ITransform* transform();

	virtual void setTargetPos(float x,float y,float z);
	virtual void getTargetPos(ola::vec3* out_pos);

protected:
	OlaLight* mLight;
	OlaTransformImpl* mTransform;
};

#endif