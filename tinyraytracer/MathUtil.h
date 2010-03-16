/*
*
*	Author : Umut Riza ERTURK
*	March 2010
*	
*/
#pragma once
#include "Definitions.h"
#include <cmath>

namespace MathUtil
{
	__forceinline Real sqrtX(Real f)
	{
		return sqrtf(f);
	}


	const static float PI = 3.1415926535897932384626433832795f;
	const static float TWO_PI = 6.283185307179586476925286766559f;

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