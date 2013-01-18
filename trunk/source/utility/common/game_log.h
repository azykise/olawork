#pragma once

#include <iostream>

//-------------------------------------------
//name: 日志系统
//desc: 向文件中写入日志
//by:	lwf 2010.8.14
//-------------------------------------------
namespace GameLog
{
	//初始化 打开文件
	//文件名: GameLog_year_month_day_hour_min_sec.txt
	void init();

	//打印一行 int
	void print_line(int);
	//打印一行 uint
	void print_line(unsigned int);
	//打印一行 64bit
	void print_line(unsigned __int64);
	//打印一行 string
	void print_line(std::string&);
	//打印一行 float
	void print_line(float);

	//日志关闭
	void shut_down();
}