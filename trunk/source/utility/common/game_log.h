#pragma once

#include <iostream>

//-------------------------------------------
//name: ��־ϵͳ
//desc: ���ļ���д����־
//by:	lwf 2010.8.14
//-------------------------------------------
namespace GameLog
{
	//��ʼ�� ���ļ�
	//�ļ���: GameLog_year_month_day_hour_min_sec.txt
	void init();

	//��ӡһ�� int
	void print_line(int);
	//��ӡһ�� uint
	void print_line(unsigned int);
	//��ӡһ�� 64bit
	void print_line(unsigned __int64);
	//��ӡһ�� string
	void print_line(std::string&);
	//��ӡһ�� float
	void print_line(float);

	//��־�ر�
	void shut_down();
}