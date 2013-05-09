#ifndef __OLA__KERNELOBJECT__H__
#define __OLA__KERNELOBJECT__H__

#include "ola_util.h"

class OlaRender;
class OlaKernelObj
{
public:
	virtual ~OlaKernelObj(){}

	virtual void updateInternal(){}

	virtual void renderInternal(OlaRender* r){}
};

#endif
