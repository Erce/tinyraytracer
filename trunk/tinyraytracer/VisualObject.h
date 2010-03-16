/*
*
*	Author : Umut Riza ERTURK
*	March 2010
*	
*/

#pragma once
#include "Definitions.h"
#include "Ray.h"
#include "Colour3f.h"
typedef struct _Material
{

	_Material()
	{
		m_rDiffuse = m_rSpecular = 0.5f;
		m_rReflectivity = 0;
		m_Colour = Colour3f(0);
	};
	_Material(Real rDiffuse, Real fReflectivity, const Colour3f& colour)
	{
		rDiffuse = abs(rDiffuse);
		fReflectivity = abs(fReflectivity);
		if (rDiffuse > 1.f)
		{
			rDiffuse = 1.f;
		}
		if (fReflectivity > 0.0f)
		{
			rDiffuse = 1.f;
		}
		m_rDiffuse = rDiffuse;
		m_rSpecular = 1.f - rDiffuse;
		m_rReflectivity = fReflectivity;
		m_Colour = colour;
	};
	Real m_rDiffuse;
	Real m_rSpecular;
	Real m_rReflectivity;

	Colour3f m_Colour;
}Material;


enum VisualObjectType
{
	VOT_NONE = 0,
	VOT_SPHERE = 1,
	VOT_PLANE = 2
};

class VisualObject
{
public:
	
	VisualObjectType	m_Type;

	Material*			m_pMaterial;


	VisualObject(void);
	VisualObject(VisualObjectType type);
	VisualObject(VisualObjectType type,  Material* pMaterial);
	~VisualObject(void)
	{
		//SAFE_DELETE(m_pMaterial);
	};

	virtual IntersectionType intersect(const Ray& ray, Real &distance) = 0;
	virtual bool intersectOutside(const Ray& ray) = 0;
	virtual Vector3 getNormal( const Vector3 &point = Vector3::VECTOR_ZERO ) = 0;

};
