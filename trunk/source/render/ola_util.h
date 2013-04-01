#ifndef _NV_UTILITY_H__
#define _NV_UTILITY_H__

#include <memory.h>
#include <stdlib.h>
#include <stdio.h>

#if defined WIN32

#define  LOGI(...) printf(__VA_ARGS__)
#define  lg(...) printf(__VA_ARGS__)

#elif defined __ANDROID_API__

#include <jni.h>
#include <android/log.h>
#define  LOG_TAG    "GLRenderJNI"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#define  lg(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)

#elif defined __APPLE__

#define  LOGI(...) printf(__VA_ARGS__)
#define  lg(...) printf(__VA_ARGS__)

#endif

#include "ola_stl.h"
#include "ola_string.h"

class OlaUtility
{
public:
	static int readStringLines(olastring& s,OlaArray<olastring>& outLines);
};

#endif
