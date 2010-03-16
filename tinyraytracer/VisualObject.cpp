#include "VisualObject.h"


VisualObject::VisualObject(void)
{
	m_Type = VOT_NONE;
}
VisualObject::VisualObject(VisualObjectType type)
{
	m_Type = type;
}
VisualObject::VisualObject(VisualObjectType type,  Material* pMaterial)
{
	m_Type = type;
	m_pMaterial = pMaterial;
}

