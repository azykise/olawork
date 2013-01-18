
#include <stdlib.h>
#include <memory>

#include "data_stream.h"

DataStream::DataStream():
m_pData(0),
m_pos(0),
m_size(0)
{

}

DataStream::DataStream(DataStream& ds):
m_pos(ds.m_pos),
m_size(ds.m_size)
{
	m_pData = malloc(m_size);
	memcpy(ds.m_pData,m_pData,m_size);
}

DataStream::DataStream(std::ifstream& inf):
m_pData(0),
m_pos(0),
m_size(0)
{
	if(inf.is_open())
	{
		inf.seekg(0,std::ios::end);
		m_size = inf.tellg();
		inf.seekg(0,std::ios::beg);

		m_pData = malloc(m_size);

		inf.read(static_cast<char*>(m_pData),m_size);
		inf.close();
	}
}

DataStream::~DataStream()
{
	if(m_pData)
	{
		free(m_pData);
		m_pData = 0;
		m_size = 0;
		m_pos = 0;
	}
}

DataStream& DataStream::operator = (DataStream& ds)
{
	m_pos = ds.m_pos;
	
	if(!m_pData)
		m_pData = malloc(ds.m_size);
	else if(m_size != ds.m_size)
		m_pData = realloc(m_pData,ds.m_size);		

	memcpy(m_pData,ds.m_pData,ds.m_size);
	m_size = ds.m_size;

	return *this;
}

void DataStream::read(void* p,unsigned int size)
{
	if(m_pos + size >= m_size)
		return;

	memcpy(p,static_cast<unsigned char*>(m_pData)+m_pos,size);
}

void DataStream::read(void* p,unsigned int pos,unsigned int size)
{
	if(pos + size >= m_size)
		return;

	memcpy(p,static_cast<unsigned char*>(m_pData)+pos,size);
}

void DataStream::read(std::ifstream& inf,unsigned int size)
{
	m_size = size;

	m_pData = m_pData ? realloc(m_pData,m_size) :  malloc(m_size);

	inf.read(static_cast<char*>(m_pData),m_size);
}

void DataStream::write(const std::string& filename,std::ofstream& of)
{
	if (!of.is_open())
		of.open(filename.c_str(),std::ios::out | std::ios::app);
	of.write(static_cast<char*>(m_pData),m_size);
}