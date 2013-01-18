#pragma once

#include "pre_define.h"

namespace Engine
{
	public ref class Math 
	{
	public:
		static float DegreeToRadian(float degree);

		static float RadianToDegree(float radian);

		static bool  IsZero(float f);

		//给一个orient四元数 返回和xyz夹角的degree
		static Vector3F^ QuaternionToEular(QuaternionF^ quat);

		//给和xyz夹角的degree 返回一个orient四元数
		static QuaternionF^ EularToQuaternion(Vector3F^ eular);

		//pi
		static const float	PI				= 3.14159265358979323846f;
		//pi * 2
		static const float	TWO_PI			= 2.0f * PI;
		//pi / 2
		static const float	HALF_PI			= 0.5f * PI;
		//pi / 4
		static const float	ONEFOURTH_PI	= 0.25f * PI;
		//e
		static const float	E				= 2.71828182845904523536f;
		//sqrt(2)
		static const float	SQRT_TWO		= 1.41421356237309504880f;
		//sqrt(3)
		static const float	SQRT_THREE		= 1.73205080756887729352f;
		//sqrt(1/2)
		static const float	SQRT_1OVER2		= 0.70710678118654752440f;
		//sqrt(1/3)
		static const float	SQRT_1OVER3		= 0.57735026918962576450f;
		//degrees to radians multiplier
		static const float	M_DEG2RAD		= PI / 180.0f;
		//radians to degrees multiplier
		static const float	M_RAD2DEG		= 180.0f / PI;
		//seconds to milliseconds multiplier
		static const float	M_SEC2MS		= 1000.0f;
		//milliseconds to seconds multiplier
		static const float	M_MS2SEC		= 0.001f;
		//huge number which should be larger than any valid number used
		static const float	INFINITY		= 1e30f;
	};
}