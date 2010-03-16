/*
*
*	Author : Umut Riza ERTURK
*	March 2010
*	
*
*	This is the pixel buffer class which
*	enables ray tracer to run in multithread
*	by protecting the pixelbuffer with mutex
*/


#include "Plane.h"


Plane::Plane(void)
{
	m_Type = VOT_PLANE;
}
Plane::Plane( const Vector3& normal, Real D )
{
	m_Type = VOT_PLANE;
	m_Normal = normal;
	m_rD = D;
	m_pMaterial = new Material();


}
Plane::Plane( const Vector3& normal, Real D, Material* mat)
{
	m_Type = VOT_PLANE;
	m_Normal = normal;
	m_rD = D;
	m_pMaterial = mat;
}

