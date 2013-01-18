#include <map>
using namespace std;

#include "game_id.h"
#include "global_time.h"
#include "random_kit.h"

typedef unsigned short obj_type;

struct obj_count
{
	unsigned int time_tag;
	unsigned short counter;
};

typedef map<obj_type,obj_count> obj_table;

class CIDMaker_Imp
{
public:
	static CIDMaker_Imp& instance()
	{
		static CIDMaker_Imp maker;
		return maker;
	}

	CIDMaker_Imp():m_inited(false)
	{
		m_table.clear();
	}

	void init()
	{
		if(m_inited)
			return;

		m_table.clear();

		m_inited = true;
	}

	game_id new_id(unsigned short _t)
	{
		game_id id = 0;
		unsigned int time_tag = _get_time_tag();
		unsigned int serial = _get_serial_number(_t,time_tag);

		game_id temp = (game_id)time_tag;
		id |= temp << 32;
		id |= serial;
		return id;
	}

	unsigned int _get_time_tag()
	{
		GbTime time_now = GlobalTime::get_current_time();

		unsigned int time_tag = 0;
		unsigned int temp = 0;
		time_tag |= (time_now.year - 2000) << 27;

		temp = time_now.month & 0xf;
		time_tag |= temp << 22;

		temp = time_now.day & 0x1f;
		time_tag |= temp << 17;

		temp = time_now.hour & 0x1f;
		time_tag |= temp << 12;

		temp = time_now.min & 0x3f;
		time_tag |= temp << 6;

		temp = time_now.sec & 0x3f;
		time_tag |= temp;

		return time_tag;
	}

	unsigned int _get_serial_number(unsigned short _t,unsigned int _tag)
	{
		obj_type type = (obj_type)_t;

		unsigned short instance_number = 0;

		obj_table::iterator i = m_table.find(type);

		if(i != m_table.end())
		{
			obj_count& c = i->second;

			if(c.time_tag != _tag)
				c.counter = 0;
			else
				c.counter++;

			instance_number = c.counter;
			c.time_tag = _tag;
		}
		else
		{
			obj_count c;
			c.time_tag = _tag;
			c.counter = 0;
			obj_table::value_type v(type,c);
			m_table.insert(v);
		}

		unsigned int serial_number = 0;

		serial_number |= _t << 16;
		serial_number |= instance_number;

		return serial_number;
	}
	
	obj_table m_table;
	bool m_inited;
};

void GameID::init()
{
	CIDMaker_Imp::instance().init();
}

game_id GameID::new_id(unsigned short _t)
{	
	return CIDMaker_Imp::instance().new_id(_t);
}

uint_id GameID::short_id(game_id id)
{
	unsigned int i = id & 0xffff;
	return i;
}