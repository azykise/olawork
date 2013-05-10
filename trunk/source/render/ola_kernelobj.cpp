#include "ola_kernelobj.h"

class OlaSmallTrunk : public OlaKernelObjTrunk
{
public:
	OlaSmallTrunk();

	virtual ~OlaSmallTrunk();

	virtual unsigned int kernelObjCount();

	virtual OlaKernelObj* kernelObj( unsigned int index );

	virtual OlaKernelObj* kernelObj( const char* id );

	virtual void enTrunk( OlaKernelObj* obj );

	virtual void removeUnusedObj();

protected:
	OlaArray<OlaKernelObj*> mKernelObjs;
};

OlaKernelObjTrunk* CreateSmallKernelObjTrunk()
{
	return new OlaSmallTrunk();
}

OlaSmallTrunk::OlaSmallTrunk()
{

}

#include <assert.h>
OlaSmallTrunk::~OlaSmallTrunk()
{
	assert( mKernelObjs.size() == 0 && "OlaSmallTrunk.mKernelObjs not empty!");
}

unsigned int OlaSmallTrunk::kernelObjCount()
{
	return mKernelObjs.size();
}

OlaKernelObj* OlaSmallTrunk::kernelObj( unsigned int index )
{
	if (index < 0 || index >= mKernelObjs.size())
	{
		return 0;
	}
	return mKernelObjs[index];
}

OlaKernelObj* OlaSmallTrunk::kernelObj( const char* id )
{
	for (unsigned int i = 0 ; i < mKernelObjs.size() ; i++)
	{
		OlaKernelObj* ko = mKernelObjs[i];
		if ( !strcmp(ko->kernelID(),id) )
		{
			return ko;
		}			
	}
	return 0;
}

void OlaSmallTrunk::enTrunk( OlaKernelObj* obj )
{
	if (!mKernelObjs.contains(obj))
	{
		mKernelObjs.push_back(obj);
	}
}

void OlaSmallTrunk::removeUnusedObj()
{
	OlaArray<OlaKernelObj*> removed_list;

	for (unsigned int i = 0 ; i < mKernelObjs.size() ; i++)
	{
		if (!mKernelObjs[i]->refCount())
		{
			removed_list.push_back(mKernelObjs[i]);
		}			
	}

	for (unsigned int i = 0 ; i < removed_list.size() ; i ++)
	{
		OlaKernelObj* ko = removed_list[i];
		std::remove(mKernelObjs.begin(), mKernelObjs.end() , ko);
		delete ko;
	}

	removed_list.clear();
}
