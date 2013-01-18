#ifndef _OLA_GEOMETRY_BUFFER_H__
#define _OLA_GEOMETRY_BUFFER_H__

#include <vector>

#include "ola_ase.h"

class OlaVBParam
{
public:
	enum VB_ELEMENT_TYPE
	{	
		ELEMENT_POS = 0,
		ELEMENT_NOR,
		ELEMENT_UV0,
		ELEMENT_TAN,
		ELEMENT_BNR,
		VB_ELEMENT_TYPE_END,
	};

	enum VBO_TYPE
	{
		VBO_STATIC,
		VBO_DYNAMIC,
	};

	static int getVertexElementStride(VB_ELEMENT_TYPE type);
};

class OlaVB
{
public:
	typedef std::vector<OlaVBParam::VB_ELEMENT_TYPE> ElementList;
public:
	OlaVB();
	~OlaVB();

	virtual void initialize(ElementList& elements,unsigned int num,OlaVBParam::VBO_TYPE vbot = OlaVBParam::VBO_STATIC);
	virtual void release() = 0;

	virtual void* lock();
	virtual void unlock() = 0;

	int elementPos(OlaVBParam::VB_ELEMENT_TYPE type);
	int elementOffset(OlaVBParam::VB_ELEMENT_TYPE type);

	int num_v();
	int stride();

	void* localData(){return mBuffer;}

	virtual void* handle() = 0;

	inline ElementList& elements(){return mElements;}
protected:
	int mElementPos[OlaVBParam::VB_ELEMENT_TYPE_END];
	int mElementOffset[OlaVBParam::VB_ELEMENT_TYPE_END];

	void* mBuffer;
	unsigned int mVertnum;
	unsigned int mVertstride;
	
	OlaVBParam::VBO_TYPE mVBOType;

	ElementList mElements;
};

class OlaIB
{
public:
	OlaIB();
	virtual ~OlaIB();

	virtual void initialize(unsigned int num);
	virtual void release() = 0;

	virtual unsigned short* lock();
	virtual void unlock() = 0;
	
	int num_i();
	int stride();

	virtual void* handle() = 0;

	void printl3();

protected:
	std::vector<unsigned short> mData;	
};

#endif