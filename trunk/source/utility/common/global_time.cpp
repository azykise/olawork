#include <windows.h>
#include <string.h>
#include "global_time.h"

#pragma warning(disable:4996)

class CGlobalTime_Imp
{
public:

	CGlobalTime_Imp():
	m_msec_start(0),m_inited(false),m_sec_start(0)
	{
		memset(&m_time_start,0,sizeof(tm));
	}

	~CGlobalTime_Imp(){}

	void init(tm* _pt)
	{
		if(m_inited)
			return;

		//ָ����һ��ʱ����Ϊ��ʼʱ��
		if(_pt)
		{
			memcpy(&m_time_start,_pt,sizeof(tm));
			m_sec_start = mktime(_pt);
		}
		//û��ָ�����ο���������ʱ��
		else
		{
			m_sec_start = time(0);
			tm* ptm = localtime(&m_sec_start);
			memcpy(&m_time_start,ptm,sizeof(tm));
		}
		//����ʱ��tick
		m_msec_start = _get_msec64();
		m_inited = true;
	}

	GbTime get_current_time()
	{
		time_m passed = get_passed_msec();

		int msec = passed % 1000;
		time_m sec = passed / 1000;

		//������ʱ����Ͼ��������������ǵ�ǰʱ�䡣
		time_t t_now = m_sec_start + (time_t)sec;

		tm tm_now = {0};
		tm* ptm = localtime(&t_now);

		if(!ptm || !m_inited)
			return GbTime();

		memcpy(&tm_now,ptm,sizeof(tm));

		GbTime time = _tm_to_GbTime(tm_now);
		time.msec = msec;

		return time;	
	}

	GbTime get_start_time()
	{
		return _tm_to_GbTime(m_time_start);
	}

	time_m get_passed_msec()
	{
		if(!m_inited)
			return 0;

		time_m msec_now = _get_msec64();

		return msec_now - m_msec_start;
	}

	inline GbTime _tm_to_GbTime(tm& _tm)
	{
		GbTime time = {0};

		time.year = _tm.tm_year + 1900;
		time.month= _tm.tm_mon + 1;
		time.day = _tm.tm_mday;
		time.hour = _tm.tm_hour;
		time.min = _tm.tm_min;
		time.sec = _tm.tm_sec;
		time.msec = 0;

		return time;
	}

	inline time_m _get_msec64()
	{
		//cpuʱ��Ƶ�� ��/�� Ŀǰһ����� 4G 2^32 
		static LARGE_INTEGER tick_rate = {0};

		//��¼cpu�ӿ��������е�ʱ�Ӵ��� 1�� 4G ��������Լ 100����
		LARGE_INTEGER ticks; 

		//��ȡcpuʱ��Ƶ��
		QueryPerformanceFrequency(&tick_rate);

		//��ȡ��cpu���������е�ʱ�Ӵ��� 
		QueryPerformanceCounter(&ticks); 

		//��ticks/rate == �ӿ����������ھ���������
		time_m seconds = ticks.QuadPart/tick_rate.QuadPart;

		//��ticks - ��*rate == ������֮���ticks
		time_m left_part = ticks.QuadPart - (tick_rate.QuadPart*seconds);

		//������֮���ticks���������������֮��ĺ�����
		time_m mseconds = left_part*1000/tick_rate.QuadPart;

		//����ܺ����� ���ֱ����ticks * 1000 / rate ���п�����1���Ժ����
		time_m ret = seconds*1000 + mseconds;

		return ret;
	}

	time_t	m_sec_start;	//seconds from 1970.1.1 00:00 to local or a given time
	tm		m_time_start;	//time when init() , localtime or a given time
	time_m	m_msec_start;	//mseconds form init
	bool	m_inited;
};

CGlobalTime_Imp& time_instance()
{
	static CGlobalTime_Imp time;
	return time;
}

namespace GlobalTime
{
	time_m get_system_tick()
	{
		return time_instance()._get_msec64();
	}

	void init(tm* _pt)
	{
		time_instance().init(_pt);
	}

	GbTime get_current_time()
	{
		return time_instance().get_current_time();
	}

	GbTime get_start_time()
	{
		return time_instance().get_start_time();
	}

	time_m get_passed_msec()
	{
		return time_instance().get_passed_msec();
	}

	void time_to_string(GbTime& _t,std::string& _s)
	{
		char c_time[256];

		sprintf(c_time,"%d/%d/%d/%d:%d:%d:%d",_t.year,_t.month,_t.day,_t.hour,_t.min,_t.sec,_t.msec);

		_s = c_time;
	}
}

