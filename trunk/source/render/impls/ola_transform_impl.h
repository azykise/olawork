#ifndef _OLA_INCLUDE_TRASNFORM_IMPLS_H__
#define _OLA_INCLUDE_TRASNFORM_IMPLS_H__

#include "../ola_util.h"
#include "../../interface/ola_engine.h"

class OlaTransformObj;

class OlaTransformImpl : public ola::ITransform
{
public:
	OlaTransformImpl();
	virtual ~OlaTransformImpl();

	OlaTransformObj* transform();

	virtual void getPosition(ola::vec3* out_pos);
	virtual void setPosition(float x,float y,float z);

	virtual void getRotation(ola::quat* out_rot);
	virtual void setRotation(float axisx,float axisy,float axisz,float degree);

protected:
	OlaTransformObj* mTransform;
};

#endif