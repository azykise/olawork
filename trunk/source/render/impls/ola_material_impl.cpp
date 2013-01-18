#include "ola_material_impl.h"

#include "../ola_material.h"
#include "../ola_resourcemng.h"

OlaMaterialImpl::OlaMaterialImpl( OlaMaterialReference* matref):
mMatRef(matref)
{
	if(mMatRef)
		mMatRef->addRef();
}

const char* OlaMaterialImpl::name()
{
	if (mMatRef)
	{
		return mMatRef->material()->name();
	}
	else
		return 0;
}


OlaMaterialImpl::~OlaMaterialImpl()
{
	if(mMatRef)
	{
		mMatRef->delRef();
		mMatRef = 0;
	}
	
}

int OlaMaterialImpl::deserialize( const ola::byte* data,int len )
{
	return -1;
}
