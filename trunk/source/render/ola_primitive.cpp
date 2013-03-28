#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "ola_render.h"
#include "ola_primitive.h"
#include "ola_util.h"

#include "ola_device.h"

OlaPrimitive::OlaPrimitive(OlaPrimitiveParam::PRIMITIVE_TYPE type):
mID(0),
mOp(0),
mIB(0),
mVB(0)
{
	mVB = GetRenderDevice()->spawnVB();
	mIB = GetRenderDevice()->spawnIB();

	mOp = new OlaRenderOp();
	mOp->vb = mVB;
	mOp->ib = mIB;
	
	mOp->worldtrans.Identity();
	mOp->drawmode = OlaRenderParam::PRIM_LINE;

	float color[] = {1.0f,1.0f,1.0f,1.0f};
	int s = sizeof(mColor);
	memcpy(mColor,color,sizeof(float) * 4);
}

OlaPrimitive::~OlaPrimitive()
{
	if(mOp)
	{
		delete mOp;
		mOp = 0;
	}

	if(mVB)
	{
		delete mVB;
		mVB = 0;
	}

	if(mIB)
	{
		delete mIB;
		mIB = 0;
	}
}

void OlaPrimitive::setIndex(int* idxs,int num)
{		
	OlaIB* ib = mIB;	
	int num_i = ib->num_i();
	if(num_i == 0 || num_i != num)
	{
		ib->release();
		ib->initialize(num);
		num_i = ib->num_i();
	}
	unsigned short* idx = ib->lock();
	for (int i = 0 ; i < num_i ; i++)
	{
		idx[i] = idxs[i];
	}
	ib->unlock();
}

void OlaPrimitive::setVertex(float* vec3s,int num)
{
	OlaVB* vb = mVB;
	int num_v = vb->num_v();
	if(num_v == 0 || num_v != num)
	{
		vb->release();
		OlaVB::ElementList elements;
		elements.push_back(OlaVBParam::ELEMENT_POS);
		vb->initialize(elements,num / 3,OlaVBParam::VBO_DYNAMIC);
		num_v = vb->num_v();
	}

	float* data = (float*)(vb->lock());
	memcpy(data,vec3s, 3 * sizeof(float) * num_v);
	vb->unlock();
}

void OlaPrimitive::setColor(float r,float g,float b,float a)
{
	mColor[0] = r; mColor[1] = g; mColor[2] = b; mColor[4] = a;
}

OlaRenderOp* OlaPrimitive::renderop()
{
	return mOp;
}