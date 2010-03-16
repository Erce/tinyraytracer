#pragma once
#include "visualobject.h"

class Sphere :
	public VisualObject
{
public:


	Vector3 m_Origin;
	Real	m_Radius;
	Real	m_rSquareRadius;
	Real	m_rInverseRadius;

	Sphere(void);
	Sphere( const Vector3& origin, Real radius);
	Sphere( const Vector3& origin, Real radius, Material* pMaterial);
	~Sphere(){VisualObject::~VisualObject();};

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

	inline IntersectionType intersect(const Ray& ray, Real &rDistance)
	{
		Vector3 dst = ray.point - m_Origin;
		Real rB = -dst.dot(ray.dir);
		Real rD = rB * rB - dst.dot(dst) + m_rSquareRadius;
		if (rD > 0)
		{
			rD = MathUtil::sqrtX(rD);
			Real r2 = rB + rD;
			if(r2 > 0)
			{
				Real r1 = rB - rD;
				if (r1 < 0)
				{
					if (r2 < rDistance)
					{
						rDistance = r2;
						return INTERSECTS_INSIDE;
					}
				}
				else
				{
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
