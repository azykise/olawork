#ifndef _OLA_SHADER_SCRIPT_PROCESS_H__

#define OSS_MAX_STRLEN 256

struct tOSS_Property
{	
	char mName[OSS_MAX_STRLEN];
	char mDesc[OSS_MAX_STRLEN];
	char mType[OSS_MAX_STRLEN];
	char mValue[OSS_MAX_STRLEN];
};

struct tOSS_RenderState
{
	char mName[OSS_MAX_STRLEN];
	char mValue0[OSS_MAX_STRLEN];
	char mValue1[OSS_MAX_STRLEN];
	char mValue2[OSS_MAX_STRLEN];
	char mValue3[OSS_MAX_STRLEN];
	char mValue4[OSS_MAX_STRLEN];
};

struct tOSS_Tag
{
	char mName[OSS_MAX_STRLEN];
	char mValue[OSS_MAX_STRLEN];
};

struct tOSS_Function
{
	char mName[OSS_MAX_STRLEN];
	char* mValue;
	int mLen;
};

#endif