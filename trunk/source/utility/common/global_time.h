#pragma once

#include <time.h>
#include <iostream>

struct GbTime
{
	int year;	
	int month;	//[1-12]
	int day;	//[1-31]
	int hour;	//[0-23]
	int min;	//[0-59]
	int sec;	//[0-59]
	int msec;	//[0-999]
};

//���뼶���ʱ���ʾ
typedef unsigned long long time_m;

//------------------------------
//name: ʱ���¼��
//desc: Ԥ�����ۿ�����100��
//by:	lwf 2010.8.15
//------------------------------
namespace GlobalTime
{
	//��ʼ��,�趨��ʼʱ��,��ָ���Ͳο���������
	void init(tm* _pt);

	//��ȡ��ǰʱ��
	GbTime get_current_time();

	//��ȡ��ʼʱ��
	GbTime get_start_time();

	//��ȡ��init�󾭹��ĺ���ʱ��
	time_m get_passed_msec();

	//��ȡ�ӿ�����cpu tick
	time_m get_system_tick();

	void time_to_string(GbTime&,std::string&);
};