/*
*
*	Author : Umut Riza ERTURK
*	March 2010
*	
*/

#include "Scene.h"

using std::vector;
Scene::Scene(void)
{
}

Scene::~Scene(void)
{

	{
		vector<VisualObject*>::iterator it = m_VisualObjects.begin();
		vector<VisualObject*>::iterator endIt = m_VisualObjects.end();
		while(it != endIt)
		{
			VisualObject *pVO = *it;
			SAFE_DELETE(pVO);
			it++;
		}
	}


	{
		vector<PointLight*>::iterator it = m_LightObjects.begin();
		vector<PointLight*>::iterator endIt = m_LightObjects.end();
		while(it != endIt)
		{
			PointLight *pLO = *it;
			SAFE_DELETE(pLO);
			it++;
		}
	}



}

void Scene::addVisualObject(VisualObject* pVisaulObject)
{
	m_VisualObjects.push_back(pVisaulObject);
}

void Scene::addLightObject(PointLight* pLight)
{
	m_LightObjects.push_back(pLight);

}

