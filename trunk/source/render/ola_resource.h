#ifndef _OLA_INCLUDE_RESOURCE_H__
#define _OLA_INCLUDE_RESOURCE_H__

class IRefCounter
{
public:
	IRefCounter():mRefCounter(0){};
	virtual ~IRefCounter(){mRefCounter = 0;}

	virtual void addRef() {mRefCounter++;}
	virtual void delRef() {if(mRefCounter > 0) mRefCounter--;}
	inline int refCount() {return mRefCounter;}

protected:
	int mRefCounter;
};

#endif