#include "ola_util.h"
#include "ola_postprocess.h"

OlaRenderTarget::OlaRenderTarget(const char* name):
OlaTexture(),
mName(name)
{

}

void OlaRenderTarget::resize(int w,int h)
{
	initialize(w,h);
}

OlaRenderTarget::~OlaRenderTarget()
{

}

static float quaddata_opengl[] =
{
	-1.0f, 1.0f,-1.0f, 0.0f, 1.0f,
	-1.0f,-1.0f,-1.0f, 0.0f, 0.0f,
	 1.0f, 1.0f,-1.0f, 1.0f, 1.0f,
	 1.0f,-1.0f,-1.0f, 1.0f, 0.0f,
	 1.0f, 1.0f,-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f, 0.0f, 0.0f 
};

static unsigned short quadidx[] = 
{
	0,1,2,3,4,5
};

OlaScreenQuad::OlaScreenQuad(const char* name,OlaScreenQuadChain* parent):
OlaSubMesh(name),
mParent(parent),
mPixelW(0),
mPixelH(0)
{
	
}

OlaScreenQuad::~OlaScreenQuad()
{
	
}

OlaScreenQuadChain::OlaScreenQuadChain()
{

}

OlaScreenQuadChain::~OlaScreenQuadChain()
{
	release();
}



void OlaScreenQuadChain::initialize()
{

}

void OlaScreenQuadChain::release()
{
	mQuadList.clear();

	OlaScreenQuadChain::ScreenQuadTable::iterator quad_i = mQuads.begin();
	while(quad_i != mQuads.end())
	{
		delete quad_i->second;
		quad_i++;
	}
	mQuads.clear();
}

OlaScreenQuad* OlaScreenQuadChain::creatScreenQuad(const char* name,OlaMaterial* mat)
{
	OlaScreenQuad* quad = new OlaScreenQuad(name,this);
	quad->setMaterial(mat);
	olastring _name = quad->name();
	mQuads[_name] = quad;

	OlaVB::ElementList elements;
	elements.push_back(OlaVBParam::VB_ELEMENT_TYPE::ELEMENT_POS);
	elements.push_back(OlaVBParam::VB_ELEMENT_TYPE::ELEMENT_UV0);
	quad->vb()->initialize(elements,6);

	void* p = quad->vb()->lock();
	memcpy(p,quaddata_opengl,sizeof(float) * 30);
	quad->vb()->unlock();

	quad->ib()->initialize(6);
	unsigned short* i = quad->ib()->lock();
	memcpy(i,quadidx,sizeof(unsigned short) * 6);
	quad->ib()->unlock();

	return quad;
}

OlaScreenQuad* OlaScreenQuadChain::getScreenQuad(const char* name)
{
	olastring _name(name);
	OlaScreenQuadChain::ScreenQuadTable::iterator i = mQuads.find(_name);
	if(i != mQuads.end())
	{
		return i->second;
	}
	else
		return 0;
}