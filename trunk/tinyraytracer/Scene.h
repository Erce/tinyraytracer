#pragma once
#include "VisualObject.h"
#include "PointLight.h"
#include <vector>
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
		const unsigned int uiSz = m_VisualObjects.size();
		for (unsigned int i = 0; i < uiSz; i++)
		{
			pTmpVisualO = m_VisualObjects[i];
			tmp = pTmpVisualO->intersect(ray, rDistance);
			if ( tmp != INTERSECTION_NO)
			{
				ret = tmp;
				pVisaulObject = pTmpVisualO;
			}
		}
		return ret;
	};

	bool anyNonLightIntersects( const Ray& rRay, Real &rDistance )
	{
		const unsigned int uiSz = m_VisualObjects.size();

		for (int i = 0; i < uiSz; i++)
		{
			VisualObject *pVisualObject = m_VisualObjects[i];
			if (pVisualObject->intersect(rRay,rDistance))
			{
				return true;
			}
		}
		return false;
	};

	void addVisualObject( VisualObject* pVisaulObject );
	void addLightObject	( PointLight* pLight );



	std::vector<VisualObject*> m_VisualObjects;

	std::vector<PointLight*> m_LightObjects;

};
