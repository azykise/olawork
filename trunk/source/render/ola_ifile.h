#ifndef _OLA_IFILE_H__
#define _OLA_IFILE_H__

#include "ola_file.h"

struct tIFileHandle;
class OlaIFile : public OlaFile
{
public:
    OlaIFile();
    
    virtual ~OlaIFile();
    
	virtual bool open(const char* filename);
    
	virtual int read(void* buffer,int size,int num);
    
	virtual int seek(long offset,int origin);
    
	virtual int getc();
    
	virtual long tell();
    
	virtual void close();
    
	virtual int len();  
protected:
    tIFileHandle* mHandle;
    //    NSData* mData;
};

#endif