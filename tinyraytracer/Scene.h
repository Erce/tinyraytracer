#pragma once
#include "VisualObject.h"
class Scene
{
public:
	Scene(void);
	~Scene(void);

	  
	inline IntersectionType getNearestIntersection( const Ray& ray, VisualObject*& pVisaulObject, Real& rDistance )
	{
		pVisaulObject = 0;
		VisualObject* pTmpVisualO;
		IntersectionType ret=INTERSECTION_NO;
		IntersectionType tmp;
		for (unsigned int i = 0; i < m_uiNumVisualObjects; i++)
		{
			pTmpVisualO = m_pVisualObjects[i];
			tmp = pTmpVisualO->intersect(ray, rDistance);
			if ( tmp != INTERSECTION_NO)
			{
				ret = tmp;
				pVisaulObject = pTmpVisualO;
			}
		}
		return ret;
	};

	void addVisualObject( VisualObject* pVisaulObject );
	void addLightObject	( VisualObject* pVisaulObject );
	bool anyNonLightIntersects( const Ray& rRay, Real &rDistance );
	VisualObject **m_pVisualObjects;
	VisualObject **m_pLightObjects;
	unsigned int m_uiNumVisualObjects;
	unsigned int m_uiNumLightObjects;
};
