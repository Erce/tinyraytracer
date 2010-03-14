#pragma once
#include "Definitions.h"
#include "Ray.h"
#include "Colour3f.h"
typedef struct _Material
{
	Real m_rDiffuse;
	Real m_RSpecular;
	Real m_rReflectivity;
	Colour3f m_RColour;
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
	bool				m_bIsLightEmiter;

	Material			m_Material;


	VisualObject(void);
	VisualObject(VisualObjectType type);
	VisualObject(VisualObjectType type,  const Material& mat);
	~VisualObject(void);

	virtual IntersectionType intersect(const Ray& ray, Real &distance) = 0;
	virtual bool intersectOutside(const Ray& ray) = 0;
	virtual Vector3 getNormal( const Vector3 &point = Vector3::ZERO ) = 0;

};
