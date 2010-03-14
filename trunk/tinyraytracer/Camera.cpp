#include "Camera.h"

Camera::Camera(const Vector3& topLeft, const Vector3& bottomLeft, const Vector3& cameraDir, Real cameraDistance , Real aspectRatio )
{
	Real height = (topLeft - bottomLeft).length();
	Real width = height * aspectRatio;
	if (cameraDistance == 0)
	{
		cameraDistance = width;
	}
	Vector3 normalisedCamDir = cameraDir.normalisedCopy();
	Vector3 left2RightDir = ((topLeft - bottomLeft).normalisedCopy().crossProduct(normalisedCamDir));
	left2RightDir *= width;
	m_NearPlane.m_BottomLeft = bottomLeft;
	m_NearPlane.m_TopLeft = topLeft;
	m_NearPlane.m_TopRight = left2RightDir + topLeft;
	m_NearPlane.m_BottomRight= left2RightDir + bottomLeft;

	m_CameraPoint = (m_NearPlane.m_BottomLeft + m_NearPlane.m_BottomRight + m_NearPlane.m_TopLeft + m_NearPlane.m_TopRight) / 4.f;
	m_CameraPoint += -cameraDistance * normalisedCamDir ;

}

Camera::~Camera(void)
{
}
