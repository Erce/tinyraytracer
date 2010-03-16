/*
*
*	Author : Umut Riza ERTURK
*	March 2010
*	
*/

#include "Camera.h"

/************************************************************************/
/* left-top, left bottom, camera direction, distance of the focus point 
/* and the aspect ratio values are needed
/************************************************************************/
Camera::Camera(const Vector3& topLeft, const Vector3& bottomLeft, const Vector3& cameraDir, Real cameraDistance , Real aspectRatio )
{
	Real height = (topLeft - bottomLeft).length();
	Real width = height * aspectRatio;
	if (cameraDistance == 0)
	{
		cameraDistance = width;
	}
	Vector3 normalisedCamDir = cameraDir.normalisedCopy();
	// we need to find the right-top and right-bottom points of the near plane
	Vector3 left2RightDir = ((topLeft - bottomLeft).normalisedCopy().crossProduct(normalisedCamDir));
	left2RightDir *= width;
	m_NearPlane.m_BottomLeft = bottomLeft;
	m_NearPlane.m_TopLeft = topLeft;
	m_NearPlane.m_TopRight = left2RightDir + topLeft;
	m_NearPlane.m_BottomRight= left2RightDir + bottomLeft;

	// camera point is the mid point of the four corners
	m_CameraPoint = (m_NearPlane.m_BottomLeft + m_NearPlane.m_BottomRight + m_NearPlane.m_TopLeft + m_NearPlane.m_TopRight) / 4.f;
	
	// put the camera's focus point behind the nead plane
	m_CameraPoint += -cameraDistance * normalisedCamDir ;

}

Camera::~Camera(void)
{
}
