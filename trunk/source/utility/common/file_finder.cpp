#include <Windows.h>

#include "file_finder.h"
#pragma warning(disable:4996)

typedef std::vector<std::string> stringList;

static char		c_buffer[MAX_PATH];
static wchar_t	w_buffer[MAX_PATH];

static std::string to_string(DWORD d)
{
	memset(c_buffer,0,MAX_PATH);
	sprintf_s(c_buffer,MAX_PATH,"%d",d);
	return std::string(c_buffer);
}

//multibyte to widechar 解决ifstream打不开中文路径问题
static std::wstring to_wstring(std::string& _s)
{
	memset(w_buffer,0,sizeof(w_buffer));
	MultiByteToWideChar(CP_ACP, 0, _s.c_str(), _s.size(), w_buffer, _s.size());
	return std::wstring(w_buffer);
}

static std::string getLastOf(const std::string& fullname,char c)
{
	size_t len = fullname.size();
	size_t idx = fullname.find_last_of(c) + 1; 
	size_t name_len = len - idx;
	return fullname.substr(idx,len);
}

static std::string getFilenameFormFullname(const std::string& fullname)
{
	return getLastOf(fullname,'\\');
}

WIN32_FIND_DATAA g_data = {0};
static bool fileExist(const std::string& fullname)
{
	HANDLE hFind;
	hFind = FindFirstFileA(fullname.c_str(), &g_data);
	if (hFind == INVALID_HANDLE_VALUE)
		return false;
	else
	{
		FindClose(hFind);
		return true;
	}
}

//根据一个WIN32_FIND_DATAA创建一个fileinfo
static FileInfo* createFileInfo(DirectoryInfo& parent,WIN32_FIND_DATAA& data)
{
	std::string filename = data.cFileName;
	std::string fullname = parent.fullname + '\\' + filename;
	FileInfo* file = new FileInfo(filename,fullname);

	SYSTEMTIME sys_time = {0};
	
	FileTimeToSystemTime(&data.ftCreationTime,&sys_time);
	file->creationTime = to_string(sys_time.wYear) + "_" + to_string(sys_time.wMonth) + "_" + to_string(sys_time.wDay) + "_" +to_string(sys_time.wHour) + "_" +to_string(sys_time.wMinute) + "_" + to_string(sys_time.wSecond);
	
	FileTimeToSystemTime(&data.ftLastWriteTime,&sys_time);
	file->writeTime = to_string(sys_time.wYear) + "_" + to_string(sys_time.wMonth) + "_" + to_string(sys_time.wDay) + "_" +to_string(sys_time.wHour) + "_" +to_string(sys_time.wMinute) + "_" + to_string(sys_time.wSecond);

	file->readOnly = data.dwFileAttributes & FILE_ATTRIBUTE_READONLY;
	file->type = getLastOf(filename,'.');
	return file;
}

//根据一个WIN32_FIND_DATAA创建一个dirinfo
static DirectoryInfo* createDirectoryInfo(DirectoryInfo& parent,WIN32_FIND_DATAA& data)
{
	std::string dirname = data.cFileName;
	std::string fullname = parent.fullname + '\\' + dirname;
	DirectoryInfo* dir = new DirectoryInfo(dirname,fullname);

	SYSTEMTIME sys_time = {0};

	FileTimeToSystemTime(&data.ftCreationTime,&sys_time);
	dir->creationTime = to_string(sys_time.wYear) + "_" + to_string(sys_time.wMonth) + "_" + to_string(sys_time.wDay) + "_" +to_string(sys_time.wHour) + "_" +to_string(sys_time.wMinute) + "_" + to_string(sys_time.wSecond);

	return dir;
}

DirectoryInfo::DirectoryInfo(const std::string& _name, const std::string& _fullname):
fullname(_fullname),
name(_name),
m_exist(false)
{
	m_exist = fileExist(fullname);
	if (m_exist)
	{
		
	}
	else
		fullname = name = "";
}

DirectoryInfo::DirectoryInfo(const std::string &_fullname):
fullname(_fullname),
m_exist(false)
{
	m_exist = fileExist(fullname);

	if (m_exist)
	{
		name = getFilenameFormFullname(fullname);
	}
	else
		fullname = "";	
}

FileList& DirectoryInfo::getFiles(const char* _cmd,bool refresh)
{
	if ( m_exist && (subFiles.size() == 0 || refresh))
	{
		subFiles.clear();
		std::string cmd(_cmd);
		std::string start_pos = fullname + "\\" + cmd;
		HANDLE hFind;
		WIN32_FIND_DATAA find_data = {0};
		hFind = FindFirstFileA(start_pos.c_str(), &find_data);
		if (hFind != INVALID_HANDLE_VALUE) 
		{
			if(!(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				subFiles.push_back(FilePtr(createFileInfo(*this,find_data)));

			while (FindNextFileA(hFind, &find_data) != 0) 
			{
				if(!(find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
					subFiles.push_back(FilePtr(createFileInfo(*this,find_data)));
			}

			FindClose(hFind);
		}
	}

	return subFiles;
}

DirectoryList& DirectoryInfo::getSubDirs(bool refresh)
{
	if ( m_exist && (subDirs.size() == 0 || refresh))
	{
		subDirs.clear();

		std::string start_pos = fullname + "\\*";
		HANDLE hFind;
		WIN32_FIND_DATAA find_data = {0};
		hFind = FindFirstFileA(start_pos.c_str(), &find_data);
		if (hFind != INVALID_HANDLE_VALUE) 
		{
			if((find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				std::string filename = find_data.cFileName;
				if(filename != "." && filename != "..")
					subDirs.push_back(DirectoryPtr(createDirectoryInfo(*this,find_data)));	
			}

			while (FindNextFileA(hFind, &find_data) != 0) 
			{
				if((find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					std::string filename = find_data.cFileName;
					if(filename == "." || filename == "..")
						continue;

					subDirs.push_back(DirectoryPtr(createDirectoryInfo(*this,find_data)));
				}
			}

			FindClose(hFind);
		}
	}
	return subDirs;
}

bool DirectoryInfo::exist(const std::string& _fullname)
{
	return fileExist(_fullname);
}

FileInfo::FileInfo(const std::string &_fullname):
fullname(_fullname),
m_exist(false)
{
	m_exist = fileExist(fullname);
	if (m_exist)
	{
		name = getFilenameFormFullname(fullname);
	}
	else
		fullname = "";
}

FileInfo::FileInfo(const std::string& _name, const std::string& _fullname):
name(_name),
fullname(_fullname),
m_exist(false)
{
	m_exist = fileExist(fullname);
	if (m_exist)
	{

	}
	else
		fullname = name = "";
}

FileInfo::~FileInfo()
{
	close();
}

std::ifstream& FileInfo::infstream()
{
	if (m_ifs.isNull())
	{
		pointer<std::ifstream> ifs(new std::ifstream());
		m_ifs = ifs;
	}
	return *m_ifs;
}

//如果文件存在才打开
std::ifstream& FileInfo::open()
{
	std::ifstream& fs = this->infstream();

	if (fs.is_open())
		fs.close();
	
	if(m_exist)
	{
		std::wstring ws = to_wstring(fullname);
		fs.open(ws.c_str(),std::ios::in | std::ios::binary);
	}

	return fs;
}

void FileInfo::close()
{
	std::ifstream& fs = this->infstream();
	if (fs.is_open())
	{
		fs.clear();
		fs.close();
	}
}

bool FileInfo::exist(const std::string& _fullname)
{
	return fileExist(_fullname);
}


