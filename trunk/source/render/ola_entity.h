#ifndef __OLA__ENTITY__H__
#define __OLA__ENTITY__H__

#include "ola_util.h"

//ola_render�н�������entity�ĸ��
//entityƫ��һ���ռ��е�ʵ�壬
//����ola_render��û��ʵ�壬ֻ����Ⱦ��������ola_renderer������Ϊ��Ⱦ�����
class OlaEntity
{
public:
	virtual ~OlaEntity(){};

	virtual void updateInternal(){};
};

#endif