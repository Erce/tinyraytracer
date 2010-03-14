#include "Scene.h"

Scene::Scene(void)
{
	m_pVisualObjects = new VisualObject* [1024];
	m_pLightObjects = new VisualObject* [16];
	m_uiNumVisualObjects = 0;
	m_uiNumLightObjects = 0;

}

Scene::~Scene(void)
{
}

void Scene::addVisualObject(VisualObject* pVisaulObject)
{
	m_pVisualObjects[m_uiNumVisualObjects++] = pVisaulObject;
}

void Scene::addLightObject(VisualObject *pVisaulObject)
{
	m_pVisualObjects[m_uiNumVisualObjects++] = pVisaulObject;
	m_pLightObjects[m_uiNumLightObjects++] = pVisaulObject;
	pVisaulObject->m_bIsLightEmiter = true;
}

bool Scene::anyNonLightIntersects( const Ray& rRay, Real &rDistance )
{
	for (int i = 0; i < m_uiNumVisualObjects; i++)
	{
		VisualObject *pVisualObject = m_pVisualObjects[i];
		if (pVisualObject->m_bIsLightEmiter)
		{
			continue;
		}

		if (pVisualObject->intersect(rRay,rDistance))
		{
			return true;
		}
	}
	return false;
}


