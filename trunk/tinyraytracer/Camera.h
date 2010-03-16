/*
*
*	Author : Umut Riza ERTURK
*	March 2010
*	
*/

#pragma once
#include "NearPlane.h"
class Camera
{
public:
	
	NearPlane	m_NearPlane;
	Vector3		m_CameraPoint;

	Camera(const Vector3& topLeft, const Vector3& bottomLeft, const Vector3& cameraDir, Real cameraDistance = 0, Real aspectRatio= 4.f/3.f  );
	~Camera(void);
};
