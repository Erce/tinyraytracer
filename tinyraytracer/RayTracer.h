#pragma once
#include "Camera.h"
#include "PixelBuffer.h"
#include "Scene.h"
#include <stack>

class RayTracer
{
public:
	
	RayTracer(unsigned int uiNumOfThreads, PixelBuffer* pPixelBuffer,Scene *pScene, Camera* pCamera, Colour3f &ambientLight, unsigned int uiTraceDepth=4, unsigned int uiRayPerPixel=1, unsigned int uiMinRayPerReflection=1, unsigned int uiMaxRayPerReflection=1);
	~RayTracer(void);

	void renderScene();



	Scene		*m_pScene;
	Camera		*m_pCamera;
	PixelBuffer	*m_pPixelBuffer;
	Colour3f	 m_AmbientLight;
	unsigned int m_uiTraceDepth;
	unsigned int m_uiRayPerPixel;
	unsigned int m_uiMinRayPerReflection;
	unsigned int m_uiMaxRayPerReflection;

	Vector3		m_vWidthStepSize;
	Vector3		m_vHeightStepSize;
	Real		m_rDivFactor;
	unsigned int m_uiNumOfPixels;
	unsigned int m_uiNumOfThreads;
	DWORD  		 m_ThreadIds[16];
	void renderPixel(std::stack<Ray> &rays, Colour3f& rColour);

};
