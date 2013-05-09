#ifndef __OLA__ENTITY__H__
#define __OLA__ENTITY__H__

#include "ola_util.h"

//ola_render中将不再有entity的概念，
//entity偏向一个空间中的实体，
//但是ola_render中没有实体，只有渲染对象，所有ola_renderer对象都是为渲染服务的
class OlaEntity
{
public:
	virtual ~OlaEntity(){};

	virtual void updateInternal(){};
};

#endif