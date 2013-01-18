#include "ola_string.h"
#include "ola_file.h"

#include <stdio.h>

struct tOlaFileHandle
{
	FILE* File;
};

OlaWFile::OlaWFile():
mHandle(0)
{
	
}

OlaWFile::~OlaWFile()
{
	close();
}
bool OlaWFile::open(const char* filename)
{
	mHandle = new tOlaFileHandle;
    mHandle->File = fopen(filename, "rb");
    if(mHandle->File == NULL) {
		olastring s("../assets/");
		s.append(filename);
		mHandle->File = fopen(s.c_str(), "rb");
    }

	return mHandle->File != 0;
}

int OlaWFile::read(void* buffer,int size,int num)
{
	if(mHandle && mHandle->File)
	{
		return fread(buffer,size,num,mHandle->File);
	}
	return 0;
}

int OlaWFile::seek(long offset,int origin)
{
	if(mHandle && mHandle->File)
	{
		return fseek(mHandle->File,offset,origin);
	}
	return 0;
}

void OlaWFile::close()
{
	if(mHandle && mHandle->File)
	{
		fclose(mHandle->File);
		mHandle->File = 0;
		delete mHandle;
		mHandle = 0;
	}	
}

int OlaWFile::getc()
{
	if(mHandle && mHandle->File)
	{
		return fgetc(mHandle->File);
	}
	return 0;
}

long OlaWFile::tell()
{
	if(mHandle && mHandle->File)
	{
		return ftell(mHandle->File);
	}
	return 0;
}

int OlaWFile::len()
{
	if(mHandle && mHandle->File)
	{
		fseek(mHandle->File, 0, SEEK_END); 
		int iLen = ftell(mHandle->File);
		fseek(mHandle->File, 0, SEEK_SET); 
		return iLen;
	}
	return 0;	
}