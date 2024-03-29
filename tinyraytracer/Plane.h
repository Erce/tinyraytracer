/*
*
*	Author : Umut Riza ERTURK
*	March 2010
*	
*/

#pragma once
#include "visualobject.h"

class Plane :
	public VisualObject
{
public:
	Vector3 m_Normal;
	Real	m_rD;

	Plane(void);
	Plane( const Vector3& normal, Real point);
	Plane( const Vector3& normal, Real point, Material* pMaterail);
	~Plane(){VisualObject::~VisualObject();};;

	__forceinline Vector3 getNormal( const Vector3 &point = Vector3::VECTOR_ZERO )
	{
		return m_Normal;
	}
	inline bool intersectOutside(const Ray& ray)
	{
		Real D = m_Normal.dot(ray.dir);
		Real tmp = (m_Normal.dot(ray.point) + m_rD);

		return (D!=0) && ((D < 0)^(tmp < 0));

	}
	__forceinline IntersectionType intersect(const Ray& ray, Real &rDistance)
	{
		Real D = m_Normal.dot(ray.dir);
		// if not parallel
		if (D != 0)
		{
			Real tmpDist = -(m_Normal.dot(ray.point) + m_rD) / D;
			// if doesn't intersects at the back of the ray's starting point
			if (tmpDist > 0 && tmpDist < rDistance )
			{
				rDistance = tmpDist;
				return INTERSECTS_OUTSIDE;
			}
		}
		return INTERSECTION_NO;	
	}

};
