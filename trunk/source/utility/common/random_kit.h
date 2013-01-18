#ifndef _INCLUDE_RANDOM_KIT_
#define _INCLUDE_RANDOM_KIT_

//------------------------------
//name: random kit
//desc: ���������
//by:	lwf 2010.9.13
//------------------------------
namespace random_kit
{
	void init(unsigned long long seed);			//��ʼ�����������

	int rand_i(int from,int to);				//����һ�� [from,to] ���������

	float rand_f(float from,float to,int accu);	//����һ�� [from,to] ����������� accu ��С����� [0,6]

	//�ú����� prob �ļ��ʷ���true probΪ[0,1] accu��С�����[0,6] 
	//eg: within_prob(0.25f,2) : ��25%�ļ��ʷ���true
	bool within_prob(float prob,int accu);		
}

#endif