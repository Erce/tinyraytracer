/*
*
*	Author : Umut Riza ERTURK
*	March 2010
*
*	This is the main ray tracer class
*/


#pragma once
#include "visualobject.h"

class Sphere :
	public VisualObject
{
public:


	Vector3 m_Origin;
	Real	m_Radius;
	Real	m_rSquareRadius;	// we will need the square radius thus it's better to precalculate
	Real	m_rInverseRadius;	// we will need the inverse radius thus it's better to precalculate

	Sphere(void);
	Sphere( const Vector3& origin, Real radius);
	Sphere( const Vector3& origin, Real radius, Material* pMaterial);
	~Sphere(){VisualObject::~VisualObject();};


	// assuming that the point is on the sphere
	__forceinline Vector3 getNormal( const Vector3 &point  )
	{
		return (point - m_Origin) * m_rInverseRadius;
	}
	inline bool intersectOutside(const Ray& ray)
	{
		Vector3 dst = ray.point - m_Origin;
		Real rB = -dst.dot(ray.dir);
		Real rD = rB * rB - dst.dot(dst) + m_rSquareRadius;
		if (rD > 0)
		{
			rB *= rB;			
			if (rB > rD )
			{
				return true;
			}			
		}

		return false;
	}

	/*
	* intersection test of the ray against the sphere
	*/
	__forceinline IntersectionType intersect(const Ray& ray, Real &rDistance)
	{
		// first find the ray between the ray start point and the origin of the sphere
		Vector3 dst = ray.point - m_Origin;
		Real rB = -dst.dot(ray.dir);
		// find determinant
		Real rD = rB * rB - dst.dot(dst) + m_rSquareRadius;

		// if determinant is smaller than zero
		// there is no intersection at all
		if (rD > 0)
		{
			rD = MathUtil::sqrtX(rD);
			Real r2 = rB + rD;

			// as rD is positive r2 is the bigger root
			// thus if r2 is less than zero then the
			// intersection point is behind the ray
			if(r2 > 0)
			{
				// r1 is the smaller root
				Real r1 = rB - rD;

				// if the r1 is less than zero then
				// the ray starts in the sphere
				if (r1 < 0)
				{
					// it is necessary for refraction calculations
					// if (r2 < rDistance)
					{
						rDistance = r2;
						return INTERSECTS_INSIDE;
					}
				}
				else
				{
					// rDistance is positive and smaller than 
					// rDistance which means the ray 
					// intersects outside of the sphere
					if (r1 < rDistance)
					{
						rDistance = r1;
						return INTERSECTS_OUTSIDE;
					}
				}

			}
		}
		return INTERSECTION_NO;	
	}


};
