#ifndef __OLA__KERNELOBJECT__H__
#define __OLA__KERNELOBJECT__H__

#include "ola_util.h"
#include "ola_refobj.h"

class OlaRender;
class OlaKernelObj : public IRefCounter
{
public:
	enum ENABLE_STATE
	{
		ES_DISABLE = 0,
		ES_ENABLE_ALL,
		ES_ENABLE_UPDATE,
		ES_ENABLE_RENDER,
	};
public:
	virtual ~OlaKernelObj(){}

	virtual const char* kernelID(){return 0;}

	virtual ENABLE_STATE enabled() {return ES_ENABLE_ALL;}

	virtual void updateInternal(float elasped){}

	virtual void renderInternal(OlaRender* r){}
};

class OlaKernelObjTrunk
{
public:	
	virtual ~OlaKernelObjTrunk(){};

	virtual unsigned int kernelObjCount() = 0;

	virtual OlaKernelObj* kernelObj( unsigned int index) = 0;

	virtual OlaKernelObj* kernelObj(const char* id) = 0;

	virtual void enTrunk(OlaKernelObj* obj) = 0;

	virtual void removeUnusedObj() = 0;
};

extern OlaKernelObjTrunk* CreateSmallKernelObjTrunk();

#endif
