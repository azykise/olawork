#include <stdio.h>
#include <assert.h>

//#define GEO_BUFFER_LOG 1

#include "ola_geobuffer.h"
#include "ola_util.h"

void printVec3(const char* s,float x,float y,float z)
{
	lg("print vec %s %.2f %.2f %.2f \n",s,x,y,z);
}

const int VERTEX_POS_STRIDE = 12;
const int VERTEX_NOR_STRIDE = 12;
const int VERTEX_UV0_STRIDE = 8;

int OlaVBParam::getVertexElementStride(VB_ELEMENT_TYPE type)
{
	switch(type)
	{
		case OlaVBParam::ELEMENT_POS:
			return 12;
		case OlaVBParam::ELEMENT_NOR:
			return 12;
		case OlaVBParam::ELEMENT_UV0:
			return 8;
		case OlaVBParam::ELEMENT_TAN:
			return 12;
		case OlaVBParam::ELEMENT_BNR:
			return 12;
	}	
	return -1;
}

OlaVB::OlaVB():
mBuffer(0),
mVertstride(0),
mVertnum(0),
mVBOType(OlaVBParam::VBO_STATIC)
{
	mElements.clear();
	memset(mElementPos,-1,OlaVBParam::VB_ELEMENT_TYPE_END * sizeof(int));
	memset(mElementOffset,-1,OlaVBParam::VB_ELEMENT_TYPE_END * sizeof(int));
}

OlaVB::~OlaVB()
{

}

void OlaVB::initialize(ElementList& elements,unsigned int num,OlaVBParam::VBO_TYPE vbot)
{
	release();

	mVertnum = num;
	mVertstride = 0;

	mElements.clear();
	for (unsigned int i = 0 ; i < elements.size() ; i++)
	{			
		OlaVBParam::VB_ELEMENT_TYPE type = elements[i];	
		mElementPos[type] = i;
		int stride = OlaVBParam::getVertexElementStride(type);
		if(stride != -1)
			mVertstride += stride;

		int offset = 0;
		for (unsigned int j = 0 ; j < i ; j++)
		{
			OlaVBParam::VB_ELEMENT_TYPE type_per = elements[j];
			int stride_per = OlaVBParam::getVertexElementStride(type_per);
			assert(stride_per != -1);
			offset += stride_per;
		}

		if(i == 0)
			mElementOffset[type] = 0;
		else if(offset != 0)
			mElementOffset[type] = offset;
		else
			mElementOffset[type] = -1;

		mElements.push_back(type);
	}

	if(mVertstride != 0 && mVertnum != 0)
	{
		mBuffer = malloc(mVertstride * mVertnum);
	}

	mVBOType = vbot;
}

void* OlaVB::lock()
{
	return mBuffer;
}

int OlaVB::elementOffset(OlaVBParam::VB_ELEMENT_TYPE type)
{
	return type == OlaVBParam::VB_ELEMENT_TYPE_END ? -1 : mElementOffset[type];
}

int OlaVB::elementPos(OlaVBParam::VB_ELEMENT_TYPE type)
{
	return type == OlaVBParam::VB_ELEMENT_TYPE_END ? -1 : mElementPos[type];
}

int OlaVB::num_v()
{
	return mVertnum;
}

int OlaVB::stride()
{
	return mVertstride;
}

OlaIB::OlaIB()
{

}

OlaIB::~OlaIB()
{

}

unsigned short* OlaIB::lock()
{
	return &(mData[0]);
}

void OlaIB::initialize(unsigned int num)
{
	release();
	mData.resize(num);
}

int OlaIB::num_i()
{
	return mData.size();
}

int OlaIB::stride()
{
	return sizeof(unsigned short);
}

void OlaIB::printl3()
{
	if(mData.size() > 3)
	{
		lg("face0 %d %d %.d \n",mData[0],mData[1],mData[2]);
	}
}
