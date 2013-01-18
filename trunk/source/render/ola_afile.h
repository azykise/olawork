#ifndef _OLA_ANDROID_FILE__
#define _OLA_ANDROID_FILE__

#include "ola_file.h"

struct AAsset;
class OlaAFile : public OlaFile
{
public:
	OlaAFile();

	virtual ~OlaAFile();

	virtual bool open(const char* filename);

	virtual int read(void* buffer,int size,int num);

	virtual int seek(long offset,int origin);

	virtual int getc();

	virtual long tell();

	virtual void close();
	
	virtual int len();

protected:
	AAsset* mFile;
};
#endif
