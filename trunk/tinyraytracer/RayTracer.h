/*
*
*	Author : Umut Riza ERTURK
*	March 2010
*
*	This is the main ray tracer class
*/
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
	unsigned int m_uiTraceDepth;	// number of maximum reflections
	unsigned int m_uiRayPerPixel;	// square root of number of rays per pixel
	unsigned int m_uiMinRayPerReflection;	// num of minimum rays after reflections
	unsigned int m_uiMaxRayPerReflection;	// num of max rays after reflections

	Vector3		m_vWidthStepSize;	// width step for ray direction calculations
	Vector3		m_vHeightStepSize;	// height step for ray direction calculations
	Real		m_rDivFactor;		// 1 / (m_uiRayPerPixel^2)
	unsigned int m_uiNumOfPixels;	// num of pixels to be rendered at a time by one thread
	unsigned int m_uiNumOfThreads;	// number of threads
	std::vector<HANDLE> m_ThreadHandles;	// thread handles
	std::vector<DWORD>  m_ThreadIds;		// thread ids

	void renderPixel(std::stack<Ray> &rays, Colour3f& rColour);	// renders one pixel by tracing the given rays, 
																// puts the result value in the rColour

};
