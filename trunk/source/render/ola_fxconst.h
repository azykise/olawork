#ifndef _OLA_INCLUDE_FX_CONST_H__
#define _OLA_INCLUDE_FX_CONST_H__

#include "math/ola_math.h"
#include "ola_light.h"

struct OlaFXConstParam
{
	enum FXCONST_TYPE
	{

		TYPE_END,
	};
};

struct OlaFXLightParam
{
	OlaLight::LIGHT_TYPE type;
	olaVec3 pos;
	olaVec3 dst;
};

//global shader const
struct OlaGlobalFXConst
{
	static const int MAX_LIGHT_NUM = 100;

	OlaGlobalFXConst():LightNum(0){}

	olaVec3 EyePos;
	olaVec3 EyeDst;

	olaMat4 ViewMat;
	olaMat4 ProjMat;

	int LightNum;
	OlaFXLightParam LightParam[MAX_LIGHT_NUM]; 
};

#endif