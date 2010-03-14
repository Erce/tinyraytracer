#include "Sphere.h"


Sphere::Sphere(void)
{
	m_bIsLightEmiter = false;
	m_Type = VOT_SPHERE;
}
Sphere::Sphere( const Vector3& origin, Real radius)
{
	m_bIsLightEmiter = false;
	m_Type = VOT_SPHERE;
	m_Origin = origin;
	m_Radius = radius;
	m_rSquareRadius = m_Radius * m_Radius;
	m_rInverseRadius = 1.f / m_Radius;

}
Sphere::Sphere( const Vector3& origin, Real radius, const Material& mat)
{
	m_bIsLightEmiter = false;
	m_Type = VOT_SPHERE;
	m_Origin = origin;
	m_Radius = radius;
	m_Material = mat;
	m_rSquareRadius = m_Radius * m_Radius;
	m_rInverseRadius = 1.f / m_Radius;

}


