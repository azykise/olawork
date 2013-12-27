#ifndef _SHADER_PROC_UTIL_H__
#define _SHADER_PROC_UTIL_H__

#include<string>
#include<vector>
#include<fstream>

extern std::string Trim(const std::string& str);
extern std::string RemoveComment(const std::string& str);
extern std::vector<std::string> Split(const std::string& str,char sp);
extern std::string ReadNextLine(std::fstream& fs);
extern std::string ReadAllText(const std::string& _filename);
extern std::string NextLine(const std::string& _str,std::string::iterator& iter);

#endif