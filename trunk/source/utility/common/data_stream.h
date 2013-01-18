#ifndef _MINI_INCLUDE_MINI_DATA_STREAM_H_
#define _MINI_INCLUDE_MINI_DATA_STREAM_H_

#include <fstream>
#include <iostream>

#include "../shared_pointer.h"

//������ ����������buffer 
//���Դ�filestream�������� Ҳ���Դ����������������ж�ȡ����
class DataStream
{
public:
	
	//û�д洢����
	DataStream();

	//��ʼ��ʱ,������һ��datastream�е����ݿ���һ��
	explicit DataStream(DataStream& ds);

	//��ʼ��ʱ,�����ļ��������е�����
	explicit DataStream(std::ifstream& inf);

	//��������
	virtual ~DataStream();

	//������һ��datastream�е����ݿ���һ��
	DataStream& operator = (DataStream& ds);

	//��datastream��size���ȵ����ݶ�ȡ��p�� datastream��posǰ��size
	void read(void* p,unsigned int size);

	//��datastream��posλ�õ�����size���ȶ�ȡ��p�� datastream��pos���ƶ�
	void read(void* p,unsigned int pos,unsigned int size);

	//���ļ����ж�ȡsize���ȵ����ݵ�datastream��
	void read(std::ifstream& inf,unsigned int size);

	//��datastream�����е�����д���ļ�
	void write(const std::string& filename,std::ofstream& of);

	//��datastream������ָ���ƶ���pos��
	inline void seek(unsigned int pos) {m_pos = pos;}

	//��ȡdatastream�ĳ���
	inline unsigned int size() const {return m_size;}

	//��ȡdatastream������ָ��λ��
	inline unsigned int tell() const {return m_pos;}

	//��ȡ����
	template<typename T> T getData(){return static_cast<T>(m_pData);}

protected:

	unsigned int m_pos;

	unsigned int m_size;

	void* m_pData;

};

typedef pointer<DataStream> DataStreamPtr;

#endif