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

//毫秒级别的时间表示
typedef unsigned long long time_m;

//------------------------------
//name: 时间记录器
//desc: 预计理论可运行100年
//by:	lwf 2010.8.15
//------------------------------
namespace GlobalTime
{
	//初始化,设定起始时间,不指定就参考本地日历
	void init(tm* _pt);

	//获取当前时间
	GbTime get_current_time();

	//获取起始时间
	GbTime get_start_time();

	//获取从init后经过的毫秒时间
	time_m get_passed_msec();

	//获取从开机起cpu tick
	time_m get_system_tick();

	void time_to_string(GbTime&,std::string&);
};