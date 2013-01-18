#ifndef _MINI_INCLUDE_FILE_SYSTEM_H_
#define _MINI_INCLUDE_FILE_SYSTEM_H_

#include <fstream>
#include <string>
#include <vector>

#include "../shared_pointer.h"

class FileInfo;
class DirectoryInfo;
struct FileFinder;

typedef pointer<DirectoryInfo> DirectoryPtr;
typedef pointer<FileInfo> FilePtr;
typedef std::vector<FilePtr> FileList;
typedef std::vector<DirectoryPtr> DirectoryList;

//文件信息类
//由一个文件完整路径创建文件信息对象 如果文件不存在 则该无效
class FileInfo
{
public:
	FileInfo(const std::string& fullname);
	FileInfo(const std::string& name, const std::string& fullname);
	~FileInfo();

	//文件是否存在
	inline bool exist(void) const {return m_exist;}

	//使用完整路径打开一个文件读取流对象(如果文件存在的话)
	std::ifstream& open();

	//关闭已经打开的文件流对象
	void close();

	//获取文件流对象 不一定是打开了的
	std::ifstream& infstream();

	//文件名
	std::string name;

	//完整路径
	std::string fullname;

	//文件后缀名
	std::string type;
	
	//文件创建时间
	std::string creationTime;

	//文件最后修改时间
	std::string writeTime;

	//文件是否只读
	bool readOnly;

	//判断一个路径下的文件是否存在
	static bool exist(const std::string& fullname);

private:
	FileInfo();
	FileInfo(const FileInfo&);
	FileInfo& operator = (const FileInfo&);	//暂时不允许拷贝 还没想好拷贝的意义 = = 

	pointer<std::ifstream> m_ifs;
	bool m_exist;
};

//目录信息类
//由一个目录完整路径 如果路径下该目录不存在 则该对象无效
class DirectoryInfo
{
public:
	DirectoryInfo(const std::string& fullname);
	DirectoryInfo(const std::string& name, const std::string& fullname);

	//目录是否存在
	inline bool exist(void) const {return m_exist;}

	//目录名
	std::string name;

	//目录完整路径
	std::string fullname;

	//目录创建时间
	std::string creationTime;

	//获取目录下的所有文件
	//cmd:		通配符
	//refresh:	是否重新查找 如果是的话就在磁盘上重新查找 否的话就用上一次查找的数据
	FileList& getFiles(const char* cmd = "*.*",bool refresh = false);

	//获取目录下的所有子目录
	//refresh:	是否重新查找 如果是的话就在磁盘上重新查找 否的话就用上一次查找的数据
	DirectoryList& getSubDirs(bool refresh = false);

	//判断一个路径下的目录是否存在
	static bool exist(const std::string& fullname);

private:
	DirectoryInfo();
	DirectoryInfo(const DirectoryInfo&);
	DirectoryInfo& operator = (const DirectoryInfo&);

	FileList subFiles;
	DirectoryList subDirs;
	bool m_exist;
};

#endif