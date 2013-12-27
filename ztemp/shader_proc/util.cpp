#include "util.h"

std::string Trim(const std::string& str) 
{
	std::string t = str;
	t.erase(0, t.find_first_not_of(" "));
	t.erase(t.find_last_not_of(" ") + 1);

	t.erase(0, t.find_first_not_of("	"));
	t.erase(0, t.find_first_not_of("	"));

	return t;
}

std::string RemoveComment(const std::string& str)
{
	unsigned int cpos = str.find("//");
	if(cpos == std::string::npos)
		return str;

	std::string s = str;
	s = s.substr(0,cpos);
	return s;
}

std::vector<std::string> Split(const std::string& str,char sp)
{
	std::string text = str;
	std::vector<std::string> result;	

	std::string::iterator i = text.begin();

	while(i != text.end())
	{
		std::string s;
		while(i != text.end())
		{
			char c = *i;
			i++;
			if(c != sp)
				s+= c;
			else
				break;				
		}
		result.push_back(s);
	}

	return result;
}

std::string ReadAllText(const std::string& _filename)
{
	char* buffer = 0;
	int len = 0;

	std::fstream fs;

	fs.open(_filename.c_str(),std::ios_base::in | std::ios_base::binary); //一定要read binary，不然会读到奇怪的结束符

	if(fs.is_open() && fs.good())
	{
		fs.seekg(0,std::ios_base::end);//要加offset0，否则seek不到end
		int len = fs.tellg();
		fs.seekg(0,std::ios_base::beg);

		buffer = new char[len + 1];
		fs.read(buffer,len);
		buffer[len] = '\0';
	}

	std::string str;
	if(buffer != 0)
	{
		str = buffer;
		delete[] buffer;
	}
	return str;
}

std::string ReadNextLine(std::fstream& fs)
{
	std::string line;
	if(fs.is_open() && fs.good() && !fs.eof())
	{
		std::getline(fs,line);
	}
	return line;
}


std::string NextLine(const std::string& _str,std::string::iterator& iter)
{
	std::string str;

	if(iter == _str.end())
		return str;

	while(iter != _str.end())
	{
		char c = *iter;
		iter++;
		if(c != '\n')
		{
			if(c != '\r')
				str += c;
		}
		else
		{
			break;
		}
	}		

	return str;
}