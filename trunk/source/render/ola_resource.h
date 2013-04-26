#ifndef _OLA_INCLUDE_RESOURCE_H__
#define _OLA_INCLUDE_RESOURCE_H__

class IRefCounter
{
public:
	IRefCounter():mRefCounter(0){};
	virtual ~IRefCounter();

	virtual void addRef() {mRefCounter++;}
	virtual void delRef() {if(mRefCounter > 0) mRefCounter--;}
	virtual int refCount() {return mRefCounter;}

protected:
	int mRefCounter;
};

extern const char DEFAULT_MATERIAL_NAME[];
extern const char DEFAULT_MATERIAL_DATA[]; 
extern const char DEFAULT_SHADER_NAME[];
extern const char DEFAULT_SHADER_DATA[];
extern const char DEFAULT_TEXTURE_NAME[];
extern const unsigned char DEFAULT_TEXTURE_DATA[];
extern const char DEFAULT_DML_NAME[];
extern const char DEFAULT_DML_DATA[];
extern const char DEFAULT_ASE_NAME[];
extern const char DEFAULT_ASE_DATA[];

#endif