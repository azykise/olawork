#pragma once

#include <string>

#include "../dMathHeader.h"

//------------------------------
//name: string kit
//desc: by lwf 2010.9.1
//------------------------------
namespace string_kit
{
	std::string to_string(int);

	std::string to_string(unsigned int);

	std::string to_string(float);

	std::string to_string(unsigned long long);

	std::string to_string(double);

	std::string to_string(char);

	std::string to_string(const char*);

	std::string to_string(wchar_t*);

	std::string to_string(std::string&);

	std::string to_string(std::wstring&);

	std::string to_string(vec3&);

	std::string to_string(::vec4&);

	std::wstring to_wstring(int);

	std::wstring to_wstring(unsigned int);

	std::wstring to_wstring(float);

	std::wstring to_wstring(unsigned long long);

	std::wstring to_wstring(double);

	std::wstring to_wstring(char);

	std::wstring to_wstring(char*);

	std::wstring to_wstring(std::string&);	

	int to_int(std::string&);

	int to_int(std::wstring&);

	float to_float(std::string&);

	float to_float(std::wstring&);

	unsigned long long to_uint64(std::string&);

	unsigned long long to_uint64(std::wstring&);

	vec3 to_vec3(std::string&);

	vec4 to_vec4(std::string&);
}

#define s_kit string_kit