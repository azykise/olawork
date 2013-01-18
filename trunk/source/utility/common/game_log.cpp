#pragma warning(disable:4996)

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include <windows.h>

#include "global_time.h"
#include "game_log.h"

class GameLog_Imp
{
public:
	static GameLog_Imp& instance()
	{
		static GameLog_Imp log;
		return log;
	}

	GameLog_Imp():m_inited(false)
	{

	}

	//关闭文件
	~GameLog_Imp()
	{
		if(m_file.is_open())
		{
			m_file<<"game system log close ！"<<endl;

			m_file.close();
		}
	}

	//打开文件，写文件头
	void init()
	{
		if(m_inited)
			return;

		GbTime start = GlobalTime::get_start_time();
		char c_time[256];
		sprintf(c_time,"Gamelog_%d_%d_%d_%d_%d_%d",start.year,start.month,start.day,start.hour,start.min,start.sec);

		m_name = c_time;

		m_name += ".txt";

		m_file.open(m_name.c_str(),std::ios::app);

		if(!m_file.is_open())
		{
			MessageBox(0,L"game log init failed",L"error",MB_OK);
			return;
		}

		m_inited = true;

		string s_time;
		
		GlobalTime::time_to_string(start,s_time);

		m_file<<"game system log , "<<s_time<<"    begin:"<<endl;
	}

	void shut_down()
	{
		if(m_file.is_open())
		{
			string s_time;
			GbTime time_now = GlobalTime::get_current_time();
			GlobalTime::time_to_string(time_now,s_time);

			m_file<<"game system log , "<<s_time<<"    close ！"<<endl;

			m_file.close();
		}
	}

	template<class T> void print_line(T& _s)
	{
		if(!m_inited)
			return;

		string s_time;
		GbTime time_now = GlobalTime::get_current_time();		
		GlobalTime::time_to_string(time_now,s_time);

		if(!m_file.is_open())
			return;

		m_file<<s_time<<"  $  "<<_s<<endl;	//空格4个字符
	}

	string  m_name;	//文件名
	fstream m_file;	//文件对象
	bool	m_inited;
};

void GameLog::init()
{
	GameLog_Imp::instance().init();
}

void GameLog::print_line(int _s)
{
	GameLog_Imp::instance().print_line(_s);
}

void GameLog::print_line(unsigned int _s)
{
	GameLog_Imp::instance().print_line(_s);
}

void GameLog::print_line(unsigned __int64 _s)
{
	GameLog_Imp::instance().print_line(_s);
}

void GameLog::print_line(std::string& _s)
{
	GameLog_Imp::instance().print_line(_s);
}

void GameLog::print_line(float _s)
{
	GameLog_Imp::instance().print_line(_s);	
}