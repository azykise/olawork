#include<stdio.h>

#import <UIKit/UIKit.h>
#include "ola_ifile.h"

struct tIFileHandle
{
    NSData* data;
};

OlaIFile::OlaIFile():
mHandle(0)
{
    
}

OlaIFile::~OlaIFile()
{
    close();
}

bool OlaIFile::open(const char* filename)
{
    mHandle = new tIFileHandle();
    
    NSString* _filename = [NSString stringWithUTF8String:filename];
    
    NSBundle* app_bundle = [NSBundle mainBundle];
    NSString* app_path = [app_bundle bundlePath];
    
    NSString* file_path = [app_path stringByAppendingString:@"/assets/"];
    NSString* file_name = [file_path stringByAppendingString:_filename];
    
//    NSLog(@"filename : %@",file_name);
    
    NSFileManager* fm = [NSFileManager defaultManager];
    
    mHandle->data = [fm contentsAtPath:file_name];
    
    if(mHandle->data == nil)
        return false;
    
    return true;
}

int OlaIFile::read(void* buffer,int size,int num)
{
    int rl = size * num;
    int dl = this->len();
    
    if(rl > dl)
        return 0;
    
    [mHandle->data getBytes:buffer length:size*num];
    return 1;
}

int OlaIFile::seek(long offset,int origin)
{
    return 0;
}

int OlaIFile::getc()
{
    return 0;
}

long OlaIFile::tell()
{
    return 0;
}

void OlaIFile::close()
{
    if(mHandle)
    {
        delete mHandle;
        mHandle = 0;
    }
}

int OlaIFile::len()
{
    int length = [mHandle->data length];
    return length;
}
