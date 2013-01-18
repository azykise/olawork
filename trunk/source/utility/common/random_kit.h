#ifndef _INCLUDE_RANDOM_KIT_
#define _INCLUDE_RANDOM_KIT_

//------------------------------
//name: random kit
//desc: 随机数工具
//by:	lwf 2010.9.13
//------------------------------
namespace random_kit
{
	void init(unsigned long long seed);			//初始化随机数种子

	int rand_i(int from,int to);				//返回一个 [from,to] 的随机整数

	float rand_f(float from,float to,int accu);	//返回一个 [from,to] 的随机浮点数 accu 是小数点后 [0,6]

	//该函数有 prob 的几率返回true prob为[0,1] accu是小数点后[0,6] 
	//eg: within_prob(0.25f,2) : 有25%的几率返回true
	bool within_prob(float prob,int accu);		
}

#endif