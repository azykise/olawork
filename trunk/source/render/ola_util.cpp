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

bool OlaUtility::FileExist( const char* fullname )
{
	FILE* f = fopen(fullname,"rb");
	if (f)
	{
		fclose(f);
		return true;
	}
	return false;
}

olastring OlaUtility::FilePathToAssetPath( const olastring& _filepath )
{
	const char* ASSET_FLAG = "assets/";

	olastring filepath = _filepath;
	filepath = olastring::toLower(filepath.accessData());
	filepath.replace("\\","/");
	
	olastring assetpath = "";

	int asset_idx = olastring::findText(filepath.c_str(),ASSET_FLAG);
	if (asset_idx != -1)
	{
		asset_idx += strlen(ASSET_FLAG);
		assetpath = filepath.mid(asset_idx,filepath.length() - asset_idx);
	}

	return assetpath;
}

olastring OlaUtility::FileFolder( const char* fullname )
{
	olastring filefullname(fullname);
	filefullname = olastring::toLower(filefullname.accessData());
	filefullname.replace("\\","/");

	olastring folder = filefullname;
	int index = filefullname.last('/');
	if (index != -1)
	{
		folder = filefullname.mid(0,index + 1);
	}
	return folder;
}
