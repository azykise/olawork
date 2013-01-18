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

//�ļ���Ϣ��
//��һ���ļ�����·�������ļ���Ϣ���� ����ļ������� �����Ч
class FileInfo
{
public:
	FileInfo(const std::string& fullname);
	FileInfo(const std::string& name, const std::string& fullname);
	~FileInfo();

	//�ļ��Ƿ����
	inline bool exist(void) const {return m_exist;}

	//ʹ������·����һ���ļ���ȡ������(����ļ����ڵĻ�)
	std::ifstream& open();

	//�ر��Ѿ��򿪵��ļ�������
	void close();

	//��ȡ�ļ������� ��һ���Ǵ��˵�
	std::ifstream& infstream();

	//�ļ���
	std::string name;

	//����·��
	std::string fullname;

	//�ļ���׺��
	std::string type;
	
	//�ļ�����ʱ��
	std::string creationTime;

	//�ļ�����޸�ʱ��
	std::string writeTime;

	//�ļ��Ƿ�ֻ��
	bool readOnly;

	//�ж�һ��·���µ��ļ��Ƿ����
	static bool exist(const std::string& fullname);

private:
	FileInfo();
	FileInfo(const FileInfo&);
	FileInfo& operator = (const FileInfo&);	//��ʱ�������� ��û��ÿ��������� = = 

	pointer<std::ifstream> m_ifs;
	bool m_exist;
};

//Ŀ¼��Ϣ��
//��һ��Ŀ¼����·�� ���·���¸�Ŀ¼������ ��ö�����Ч
class DirectoryInfo
{
public:
	DirectoryInfo(const std::string& fullname);
	DirectoryInfo(const std::string& name, const std::string& fullname);

	//Ŀ¼�Ƿ����
	inline bool exist(void) const {return m_exist;}

	//Ŀ¼��
	std::string name;

	//Ŀ¼����·��
	std::string fullname;

	//Ŀ¼����ʱ��
	std::string creationTime;

	//��ȡĿ¼�µ������ļ�
	//cmd:		ͨ���
	//refresh:	�Ƿ����²��� ����ǵĻ����ڴ��������²��� ��Ļ�������һ�β��ҵ�����
	FileList& getFiles(const char* cmd = "*.*",bool refresh = false);

	//��ȡĿ¼�µ�������Ŀ¼
	//refresh:	�Ƿ����²��� ����ǵĻ����ڴ��������²��� ��Ļ�������һ�β��ҵ�����
	DirectoryList& getSubDirs(bool refresh = false);

	//�ж�һ��·���µ�Ŀ¼�Ƿ����
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