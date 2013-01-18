#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <Windows.h>
#include "string_kit.h"

typedef std::string  string;
typedef std::wstring wstring;
#pragma warning(disable:4996)

//------------------------------
//name: 
//parm: 
//desc: 
//------------------------------

const int MAX_SIZE = 128;
const unsigned int UINT32_MAX = 0xffffffff;
const unsigned long long UINT64_MAX = 0xffffffffffffffff;
const unsigned long long UINT64_HALF= 0xffffffff;


//------------------------------
//name: 
//parm: 
//desc: 
//------------------------------
string string_kit::to_string(int _i)
{
	string s;
	char temp[MAX_SIZE] = {};
	sprintf(temp,"%d",_i);
	s = temp;
	return s;
}
//------------------------------
//name: 
//parm: 
//desc: 
//------------------------------
string string_kit::to_string(unsigned int _u)
{
	string s;
	char temp[MAX_SIZE] = {0};
	sprintf(temp,"%u",_u);
	s = temp;
	return s;
}
//------------------------------
//name: 
//parm: 
//desc: 
//------------------------------
string string_kit::to_string(float _f)
{
	string s;
	char temp[MAX_SIZE] = {};
	sprintf(temp,"%f",_f);
	s = temp;
	return s;
}
//------------------------------
//name: 
//parm: 
//desc: 
//------------------------------
string string_kit::to_string(unsigned long long _ull)
{
	char temp[MAX_SIZE] = {0};
	sprintf(temp,"%I64u",_ull);
	string s(temp);
	return s;
}
//------------------------------
//name: 
//parm: 
//desc: 
//------------------------------
string string_kit::to_string(double _d)
{
	string s;
	char temp[MAX_SIZE] = {};
	sprintf(temp,"%lf",_d);
	s = temp;
	return s;
}
//------------------------------
//name: 
//parm: 
//desc: 
//------------------------------
string string_kit::to_string(char _c)
{
	string s;
	char temp[MAX_SIZE] = {};
	sprintf(temp,"%s",_c);
	s = temp;
	return s;
}
//------------------------------
//name: 
//parm: 
//desc: 
//------------------------------
std::string string_kit::to_string(const char* _s)
{
	return std::string(_s);
}
//------------------------------
//name: 
//parm: 
//desc: 
//------------------------------
string string_kit::to_string(wchar_t* _ws)
{
	string s;
	char temp[MAX_SIZE] = {};	
	wcstombs(temp,_ws,MAX_SIZE);
	s = temp;
	return s;
}
//------------------------------
//name: 
//parm: 
//desc: 
//------------------------------
string string_kit::to_string(std::wstring& _ws)
{
	string s;
	char temp[MAX_SIZE] = {};	
	wcstombs(temp,_ws.c_str(),MAX_SIZE);
	s = temp;
	return s;
}
//------------------------------
//name: 
//parm: 
//desc: 
//------------------------------
string string_kit::to_string(vec3& v)
{
	string s;
	string x = to_string(v.x);
	string y = to_string(v.y);
	string z = to_string(v.z);
	s = "(" + x + "," + y + "," + z + ")";
	return s;
}
//------------------------------
//name: 
//parm: 
//desc: 
//------------------------------
std::string string_kit::to_string(std::string& s)
{
	return s;
}
//------------------------------
//name: 
//parm: 
//desc: 
//------------------------------
string string_kit::to_string(vec4& v)
{
	string s;
	string x = to_string(v.x);
	string y = to_string(v.y);
	string z = to_string(v.z);
	string w = to_string(v.w);
	s = "(" + x + "," + y + "," + z + "," + w + ")";
	return s;
}

//------------------------------
//name: 
//parm: 
//desc: 
//------------------------------
wstring string_kit::to_wstring(int _i)
{
	wstring s;
	wchar_t temp[MAX_SIZE] = {};
	swprintf(temp,L"%d",_i);
	s = temp;
	return s;
}
//------------------------------
//name: 
//parm: 
//desc: 
//------------------------------
wstring string_kit::to_wstring(unsigned int _u)
{
	wstring s;
	wchar_t temp[MAX_SIZE] = {};
	swprintf(temp,L"%u",_u);
	s = temp;
	return s;
}
//------------------------------
//name: 
//parm: 
//desc: 
//------------------------------
wstring string_kit::to_wstring(float _f)
{
	wstring s;
	wchar_t temp[MAX_SIZE] = {};
	swprintf(temp,L"%f",_f);
	s = temp;
	return s;
}
//------------------------------
//name: 
//parm: 
//desc: 
//------------------------------
wstring string_kit::to_wstring(unsigned long long _ull)
{
	wstring s;
	wchar_t temp[MAX_SIZE] = {};
	swprintf(temp,L"%I64u",_ull);
	s = temp;
	return s;
}
//------------------------------
//name: 
//parm: 
//desc: 
//------------------------------
wstring string_kit::to_wstring(double _d)
{
	wstring s;
	wchar_t temp[MAX_SIZE] = {};
	swprintf(temp,L"%lf",_d);
	s = temp;
	return s;
}
//------------------------------
//name: 
//parm: 
//desc: 
//------------------------------
wstring string_kit::to_wstring(char _c)
{
	wstring s;
	wchar_t temp[MAX_SIZE] = {};
	swprintf(temp,L"%s",_c);
	s = temp;
	return s;
}
//------------------------------
//name: 
//parm: 
//desc: 
//------------------------------
wstring string_kit::to_wstring(char* _s)
{
	return to_wstring(std::string(_s));
}
//------------------------------
//name: 
//parm: 
//desc: 
//------------------------------
wstring string_kit::to_wstring(std::string& _s)
{
	wchar_t temp[MAX_SIZE] = {};	
	MultiByteToWideChar(CP_ACP, 0, _s.c_str(), _s.size(), temp, _s.size());
	return wstring(temp);
}
//------------------------------
//name: 
//parm: 
//desc: 
//------------------------------
int string_kit::to_int(std::string& _s)
{
	return atoi(_s.c_str());
}
//------------------------------
//name: 
//parm: 
//desc: 
//------------------------------
int string_kit::to_int(std::wstring& _ws)
{
	return _wtoi(_ws.c_str());
}
//------------------------------
//name: 
//parm: 
//desc: 
//------------------------------
float string_kit::to_float(std::string& _s)
{
	return (float)atof(_s.c_str());
}
//------------------------------
//name: 
//parm: 
//desc: 
//------------------------------
float string_kit::to_float(std::wstring& _ws)
{
	return (float)_wtof(_ws.c_str());
}
//------------------------------
//name: 
//parm: 
//desc: 
//------------------------------
unsigned long long string_kit::to_uint64(std::string& _s)
{
	return _strtoui64(_s.c_str(),0,10);
}
//------------------------------
//name: 
//parm: 
//desc: 
//------------------------------
unsigned long long string_kit::to_uint64(std::wstring& _ws)
{
	return _wcstoui64(_ws.c_str(),0,10);
}
//------------------------------
//name: 
//parm: 
//desc: (1.0,2.0,3.0)
//------------------------------
vec3 string_kit::to_vec3(std::string& s)
{
	vec3 v;
	v.x = v.y = v.z = 0;
	size_t len = s.length();
	if(s.at(0) == '(' && s.at(len-1) == ')')
	{
		bool fail = false;

		int x_begin = 1;
		int x_end = s.find(',',x_begin);
		int x_len = x_end - x_begin;
		if(x_len > 0)	
		{
			string x_s = s.substr(x_begin,x_len);
			v.x = to_float(x_s);
		}
		else
			fail = true;

		int y_begin = x_end + 1;
		int y_end   = s.find(',',y_begin);
		int y_len   = y_end - y_begin;
		if(y_len <= 0)	
		{
			string y_s = s.substr(y_begin,y_len);
			v.y = to_float(y_s);
		}
		else
			fail = true;

		int z_begin = y_end + 1;
		int z_end   = s.find(')',z_begin);
		int z_len   = z_end - z_begin;
		if(z_len > 0)	
		{
			string z_s = s.substr(z_begin,z_len);	
			v.z = to_float(z_s);
		}
		else
			fail = true;
		
		if(fail)
			v.x = v.y = v.z = 0;
	}
	
	return v;
}
//------------------------------
//name: 
//parm: 
//desc: 
//------------------------------
vec4 string_kit::to_vec4(std::string& s)
{
	vec4 v;
	v.x = v.y = v.z = v.w = 0;
	size_t len = s.length();
	if(s.at(0) == '(' && s.at(len-1) == ')')
	{
		bool fail = false;

		int x_begin = 1;
		int x_end = s.find(',',x_begin);
		int x_len = x_end - x_begin;
		if(x_len > 0)	
		{
			string x_s = s.substr(x_begin,x_len);
			v.x = to_float(x_s);
		}
		else
			fail = true;

		int y_begin = x_end + 1;
		int y_end   = s.find(',',y_begin);
		int y_len   = y_end - y_begin;
		if(y_len > 0)	
		{
			string y_s = s.substr(y_begin,y_len);
			v.y = to_float(y_s);
		}
		else
			fail = true;

		int z_begin = y_end + 1;
		int z_end   = s.find(',',z_begin);
		int z_len   = z_end - z_begin;
		if(z_len > 0)	
		{
			string z_s = s.substr(z_begin,z_len);	
			v.z = to_float(z_s);
		}
		else
			fail = true;

		int w_begin = z_end + 1;
		int w_end   = s.find(')',w_begin);
		int w_len   = w_end - w_begin;
		if(w_len > 0)	
		{
			string w_s = s.substr(w_begin,w_len);	
			v.w = to_float(w_s);
		}
		else
			fail = true;
		
		if(fail)
			v.x = v.y = v.z = v.w = 0;
	}
	
	return v;
}