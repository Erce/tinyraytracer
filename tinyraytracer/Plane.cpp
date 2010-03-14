#include "Plane.h"


Plane::Plane(void)
{
	m_bIsLightEmiter = false;
	m_Type = VOT_PLANE;
}
Plane::Plane( const Vector3& normal, Real D )
{
	m_bIsLightEmiter = false;
	m_Type = VOT_PLANE;
	m_Normal = normal;
	m_rD = D;

}
Plane::Plane( const Vector3& normal, Real D, const Material& mat)
{
	m_bIsLightEmiter = false;
	m_Type = VOT_PLANE;
	m_Normal = normal;
	m_rD = D;
	m_Material = mat;
}

