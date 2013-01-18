#ifndef _MINI_INCLUDE_MINI_DATA_STREAM_H_
#define _MINI_INCLUDE_MINI_DATA_STREAM_H_

#include <fstream>
#include <iostream>

#include "../shared_pointer.h"

//数据流 单纯的数据buffer 
//可以从filestream接收数据 也可以从其它数据流对象中读取数据
class DataStream
{
public:
	
	//没有存储数据
	DataStream();

	//初始化时,将另外一个datastream中的数据拷贝一份
	explicit DataStream(DataStream& ds);

	//初始化时,读入文件流中所有的数据
	explicit DataStream(std::ifstream& inf);

	//销毁数据
	virtual ~DataStream();

	//将另外一个datastream中的数据拷贝一份
	DataStream& operator = (DataStream& ds);

	//将datastream中size长度的数据读取到p中 datastream的pos前移size
	void read(void* p,unsigned int size);

	//将datastream中pos位置的数据size长度读取到p中 datastream的pos不移动
	void read(void* p,unsigned int pos,unsigned int size);

	//从文件流中读取size长度的数据到datastream中
	void read(std::ifstream& inf,unsigned int size);

	//将datastream中所有的数据写入文件
	void write(const std::string& filename,std::ofstream& of);

	//将datastream的数据指针移动到pos处
	inline void seek(unsigned int pos) {m_pos = pos;}

	//获取datastream的长度
	inline unsigned int size() const {return m_size;}

	//获取datastream的数据指针位置
	inline unsigned int tell() const {return m_pos;}

	//获取数据
	template<typename T> T getData(){return static_cast<T>(m_pData);}

protected:

	unsigned int m_pos;

	unsigned int m_size;

	void* m_pData;

};

typedef pointer<DataStream> DataStreamPtr;

#endif