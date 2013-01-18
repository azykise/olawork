#ifndef _OLA_FILE_READ_H__
#define _OLA_FILE_READ_H__

class OlaFile
{
public:
	virtual bool open(const char* filename) = 0;

	virtual int read(void* buffer,int size,int num) = 0;

	virtual int seek(long offset,int origin)= 0;

	virtual int getc() = 0;

	virtual long tell() = 0;

	virtual void close() = 0;

	virtual int len() = 0;
};

struct tOlaFileHandle;
class OlaWFile : public OlaFile
{
public:
	OlaWFile();

	virtual ~OlaWFile();

	virtual bool open(const char* filename);

	virtual int read(void* buffer,int size,int num);

	virtual int seek(long offset,int origin);

	virtual int getc();

	virtual long tell();

	virtual void close();

	virtual int len();

protected:
	tOlaFileHandle* mHandle;
};

#endif
