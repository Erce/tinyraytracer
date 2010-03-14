#pragma once
#include "Definitions.h"
#include <cmath>

namespace Math
{
	__forceinline Real sqrtX(Real f)
	{
		return sqrtf(f);
	}

//	inline Real invSqrt(Real f)
//	{
//		return 1.f / sqrtX(f);
///*
//		float xhalf = 0.5f*f;
//		int i = *(int*)&f;
//		i = 0x5f3759df - (i>>1);
//		f = *(float*)&i;
//		f *= (1.5f - xhalf*f*f);
//		f *= (1.5f - xhalf*f*f);
//		return (f*(1.5f - xhalf*f*f));	 
//*/
//	}
};