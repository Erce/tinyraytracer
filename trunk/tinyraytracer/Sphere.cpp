#include "Sphere.h"


Sphere::Sphere(void)
{
	m_Type = VOT_SPHERE;
	SAFE_DELETE(m_pMaterial);
}
Sphere::Sphere( const Vector3& origin, Real radius)
{
	m_Type = VOT_SPHERE;
	m_Origin = origin;
	m_Radius = radius;
	m_rSquareRadius = m_Radius * m_Radius;
	m_rInverseRadius = 1.f / m_Radius;
	m_pMaterial = new Material();

}
Sphere::Sphere( const Vector3& origin, Real radius, Material* pMaterial)
{
	m_Type = VOT_SPHERE;
	m_Origin = origin;
	m_Radius = radius;
	m_pMaterial = pMaterial;
	m_rSquareRadius = m_Radius * m_Radius;
	m_rInverseRadius = 1.f / m_Radius;

}


