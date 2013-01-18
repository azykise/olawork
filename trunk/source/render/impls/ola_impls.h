#ifndef _OLA_INCLUDE_IMPLS_COMMON_H__
#define _OLA_INCLUDE_IMPLS_COMMON_H__

#define _OLA_TOSTRING(SBO) #SBO

template<class T>
class _OlaRCImpls
{
public:

	_OlaRCImpls(const char* type_name):
	mImpler(0),
	mTypename(type_name)
	{
		mImpler = new T();
	}

	virtual ~_OlaRCImpls()
	{
		if(mImpler != 0)
		{
			delete mImpler;
			mImpler = 0;
		}
	}

protected:
	char* mTypename;
	T* mImpler;
};

#endif