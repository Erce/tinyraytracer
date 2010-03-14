#pragma once
#include "Vector3.h"
class Ray
{
public:
	Ray(void){};
	Ray(const Vector3& dir, const Vector3& point )
	{
		this->dir = dir;
		this->point = point;
	}
		
	~Ray(void){};

	Vector3 dir;
	Vector3 point;

};
