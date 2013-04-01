#pragma warning(disable:4996)

#include "ola_util.h"

const char NEW_LINE = '\n';

int OlaUtility::readStringLines( olastring& s,OlaArray<olastring>& outLines )
{
	const char* c_str = s.c_str();
	int pos = 0;
	int idx = 0;

	int len = s.length() + 1;

	while(pos != len)
	{
		if(c_str[pos] == NEW_LINE)
		{
			int l = pos - 1 - idx;
			outLines.push_back(s.mid(idx,l));
			idx = pos + 1;
		}
		else if(pos == len - 1)
		{
			int l = pos - idx;
			outLines.push_back(s.mid(idx,l));
			idx = pos;
		}
		pos++;
	}

	return (int)outLines.size();
}
