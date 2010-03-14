#include "VisualObject.h"


VisualObject::VisualObject(void)
{
	m_bIsLightEmiter = false;
	m_Type = VOT_NONE;
}
VisualObject::VisualObject(VisualObjectType type)
{
	m_bIsLightEmiter = false;
	m_Type = type;
}
VisualObject::VisualObject(VisualObjectType type,  const Material& mat)
{
	m_bIsLightEmiter = false;
	m_Type = type;
	m_Material = mat;
}
VisualObject::~VisualObject(void)
{
}

