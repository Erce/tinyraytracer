/*
*
*	Author : Umut Riza ERTURK
*	March 2010
*
*	This is the main ray tracer class
*/

#pragma once
#include "VisualObject.h"
#include "PointLight.h"
#include <vector>
class Scene
{
public:
	Scene(void);
	~Scene(void);

	  
	/*
	*	finds nearest intersection point smaller than rDistance and updates the rDistance with the new value
	*	also updates pVisaulObject with the intersection object
	*	this function is the most important function in terms of efficiency
	*	in the future regular grids will be used in order to fasten up ray tracing
	*/
	inline IntersectionType getNearestIntersection( const Ray& ray, VisualObject*& pVisaulObject, Real& rDistance )
	{
		pVisaulObject = 0;
		VisualObject* pTmpVisualO;
		IntersectionType ret=INTERSECTION_NO;
		IntersectionType tmp;
		const unsigned int uiSz = m_VisualObjects.size();
		// 
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

	/*
	* Finds out if any non light objects intersects with the ray under rDistance
	*/
	bool anyNonLightIntersects( const Ray& rRay, Real &rDistance )
	{
		const unsigned int uiSz = m_VisualObjects.size();

		for (unsigned int i = 0; i < uiSz; i++)
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
