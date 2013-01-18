#include "tool.h"

namespace Engine
{
	float Math::DegreeToRadian(float degree)
	{
		return degree*M_DEG2RAD;
	}

	float Math::RadianToDegree(float radian)
	{
		return radian*M_RAD2DEG;
	}

	bool Math::IsZero(float f)
	{
		return ((*(const unsigned long *)&(f)) & ~(1<<31)) == 0;
	}

	Vector3F^ Math::QuaternionToEular(QuaternionF^ quat)
	{
		Vector3F^ eular = gcnew Vector3F(0.0f,0.0f,0.0f);


		return eular;
	}

	QuaternionF^ Math::EularToQuaternion(Vector3F^ eular)
	{
		QuaternionF^ quat = gcnew QuaternionF(1.0f,0.0f,0.0f,0.0f);
		return quat;
	}
}