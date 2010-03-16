/*
*
*	Author : Umut Riza ERTURK
*	March 2010
*	
*
*/

#include <windows.h>
#include "RayTracer.h"
#include "Sphere.h"
#include "PointLight.h"
#include "MathUtil.h"

#include <stack>


static RayTracer *gs_pRayTracer = 0;
RayTracer::RayTracer(unsigned int uiNumOfThreads, PixelBuffer* pPixelBuffer,Scene *pScene, Camera* pCamera, Colour3f &ambientLight, unsigned int uiTraceDepth, unsigned int uiRayPerPixel, unsigned int uiMinRayPerReflection, unsigned int uiMaxRayPerReflection)
{
	m_pPixelBuffer = pPixelBuffer;
	m_pScene = pScene;
	m_pCamera = pCamera;
	m_AmbientLight = ambientLight;
	m_uiMaxRayPerReflection = uiMaxRayPerReflection;
	m_uiMinRayPerReflection = uiMinRayPerReflection;
	m_uiRayPerPixel = uiRayPerPixel;
	m_uiTraceDepth = uiTraceDepth;

	m_vWidthStepSize = (m_pCamera->m_NearPlane.m_TopRight - m_pCamera->m_NearPlane.m_TopLeft ) / (Real)(m_pPixelBuffer->m_uiWidth * m_uiRayPerPixel );
	m_vHeightStepSize = (m_pCamera->m_NearPlane.m_BottomLeft - m_pCamera->m_NearPlane.m_TopLeft ) / (Real)(m_pPixelBuffer->m_uiHeight * m_uiRayPerPixel);
	m_rDivFactor = 1.f / ((Real)m_uiRayPerPixel * (Real)m_uiRayPerPixel);
	m_uiNumOfPixels = m_pPixelBuffer->m_uiBufferChunkSizeToBeRendered;
	gs_pRayTracer = this;
	m_uiNumOfThreads = uiNumOfThreads;


}

RayTracer::~RayTracer(void)
{

	// terminates threads and close thread handles
	for (int i =0; i < m_uiNumOfThreads; i++)
	{
		TerminateThread(m_ThreadHandles[i],0);
		CloseHandle(m_ThreadHandles[i]);

	}
	// delete everything
	SAFE_DELETE(m_pScene);
	SAFE_DELETE(m_pPixelBuffer);
	SAFE_DELETE(m_pCamera);

}

/************************************************************************/
/* Iterative form of ray tracing                                        */
/************************************************************************/
void RayTracer::renderPixel(std::stack<Ray> &rays, Colour3f& rColour)
{
	int iDepth=0;
	Real rCumulativeReflect=1.f;	// reflection multiplier for a ray after reflections
	bool bNoIntersection = true;
	while(!rays.empty())
	{
		Colour3f tmpColour(0);
		iDepth++;
		VisualObject *pVisaulObject = 0;
		Real rDistance = MAX_FLOAT;
		Ray rRay = rays.top();
		rays.pop();
		// find the nearest intersection point and the distance
		IntersectionType intersectionType = m_pScene->getNearestIntersection(rRay,pVisaulObject, rDistance );

		// if there is an intersection within the given distance
		if (pVisaulObject)
		{	
			bNoIntersection = false;
			Vector3& rRayDir = rRay.dir;

			// shift the ray a bit by multiplying with EPSILON
			// so the point wont be inside or on the intersected object
			// if the intersection is inside we need to push forward a bit
			// if the intersection is outside we need to pull behind a bit
			Vector3 vIntersectionPoint = rRay.point + rRayDir * (rDistance + intersectionType * EPSILON);

			Vector3 vNormalAtIntersection = pVisaulObject->getNormal(vIntersectionPoint);

			const unsigned int uiSz = m_pScene->m_LightObjects.size();
			PointLight *pLight ;
			
			std::vector<PointLight*>& lightObjects = m_pScene->m_LightObjects;
			// shading computation
			// look if there is any object between the intersection point
			// and each light source in order calculate shading of the point
			for (int k = 0; k < uiSz; k++)
			{
				pLight = lightObjects[k];

				Real rLightSourceDistance;
				Real rLightSourceDistanceSQ;

				Vector3 vDirLight = pLight->m_Position - vIntersectionPoint;
				rLightSourceDistanceSQ = vDirLight.squaredLength();
				rLightSourceDistance = MathUtil::sqrtX(rLightSourceDistanceSQ);
				
				// normalise the dir vector
				vDirLight /= rLightSourceDistance;
				Real rBrightnessFactor = 1.f / (rLightSourceDistanceSQ) ;// L = C/(d^2)

				// if the shading is below a treshold
				if (rBrightnessFactor > EPSILON)
				{
					// if there is no obstacle between the light and the intersection point
					if( m_pScene->anyNonLightIntersects(Ray(vDirLight,vIntersectionPoint),rLightSourceDistance) == false)
					{

						Real rNormalDotLightDir = vNormalAtIntersection.dot(vDirLight);
						//  diffuse lighting
						Colour3f &cLightColour = pLight->m_Colour;
						Colour3f &cVisaulObjectColour = pVisaulObject->m_pMaterial->m_Colour;
						if(rNormalDotLightDir > 0)
						{
							tmpColour += (rBrightnessFactor * rNormalDotLightDir * pVisaulObject->m_pMaterial->m_rDiffuse)
								* (cVisaulObjectColour * cLightColour);

						}

						// specular lighting
						Vector3 vTmp = vDirLight - ( (2.f * rNormalDotLightDir) * vNormalAtIntersection);
						Real rDot = vTmp.dot(rRayDir);
						if (rDot > 0)
						{
							tmpColour += (powf(rDot,16) * pVisaulObject->m_pMaterial->m_rSpecular * rBrightnessFactor) * cLightColour;
						}
					}	
				}
				
			}

			// calculate the colour value after reflections
			rColour += tmpColour * rCumulativeReflect;

			// reflected ray computation
			if(iDepth<m_uiTraceDepth)
			{
				// push the new ray to the stack
				// to be calculated
				// also update the reflectivity
				Vector3 vReflectedRayDir = rRayDir - (2.f * rRayDir.dot(vNormalAtIntersection)) * vNormalAtIntersection;
				vReflectedRayDir.normalise();
				rays.push(Ray(vReflectedRayDir,vIntersectionPoint + vReflectedRayDir * EPSILON));
				rCumulativeReflect *= pVisaulObject->m_pMaterial->m_rReflectivity;
				iDepth++;
			}
		}		
	}
	if (bNoIntersection)
	{
		rColour = m_AmbientLight;
	}
}

/************************************************************************/
/* WIN API function for render thread                                   */
/************************************************************************/
DWORD WINAPI renderThread(LPVOID param)
{
	// fetching the necessary values from the ray tracer to fasten up
	Pixel* pixelsToBeRendered;
	unsigned int uiStartX;
	unsigned int uiStartY;
	Colour3f colourOfPixel = gs_pRayTracer->m_AmbientLight;
	Vector3 &vrTopLeft = gs_pRayTracer->m_pCamera->m_NearPlane.m_TopLeft ;
	Vector3 vStartPoint;
	Vector3 &vrCameraPoint = gs_pRayTracer->m_pCamera->m_CameraPoint;
	unsigned int uiRayPerPixel = gs_pRayTracer->m_uiRayPerPixel;
	unsigned int uiNumOfPixels = gs_pRayTracer->m_uiNumOfPixels;
	const Vector3 &vWidthStepSize = gs_pRayTracer->m_vWidthStepSize;
	const Vector3 &vHeightStepSize = gs_pRayTracer->m_vHeightStepSize;
	Real rDivFactor = gs_pRayTracer->m_rDivFactor;
	Ray ray;
	ray.point = vrCameraPoint;

	std::stack<Ray> rays;
	// loop until 
	while(1)
	{
		// get the pixels to be rendered
		gs_pRayTracer->m_pPixelBuffer->getNextPixels( pixelsToBeRendered, uiStartX, uiStartY );
		
		// find the start point for the rendering
		vStartPoint =  vrTopLeft + (vWidthStepSize * (Real)uiStartX * uiRayPerPixel ) + (vHeightStepSize * (Real)uiStartY * uiRayPerPixel) ;
		
		// trace all the pixels in the batch
		for(unsigned int uiPixelIndex = 0 ; uiPixelIndex < uiNumOfPixels; uiPixelIndex++)
		{
			colourOfPixel =  Colour3f(0);/*gs_pRayTracer->m_AmbientLight*/;

			// trace all the rays in the pixel
			for(unsigned int uiRayPerPixelX = 0; uiRayPerPixelX < uiRayPerPixel; uiRayPerPixelX++ )
			{
				Vector3 vTmpStartPoint = vStartPoint;
				for(unsigned int uiRayPerPixelY = 0; uiRayPerPixelY < uiRayPerPixel; uiRayPerPixelY++ )
				{
					ray.dir = (vTmpStartPoint - vrCameraPoint);
					ray.dir.normalise();
					rays.push(ray);
					Colour3f colourOfRender(0);
					gs_pRayTracer->renderPixel(rays,colourOfRender);
					colourOfPixel = colourOfRender;
					
					// move forward
					vTmpStartPoint += vWidthStepSize;
				}
				// move down
				vTmpStartPoint += vHeightStepSize;
			}
			vStartPoint += vWidthStepSize * uiRayPerPixel;
			
			// convert the colour3f value to pixel value
			clampColour3fToPixel(colourOfPixel * rDivFactor, *pixelsToBeRendered);
			pixelsToBeRendered++;
		}
	}	
	return 0;
}


void RayTracer::renderScene()
{
	for(int i = 0; i < m_uiNumOfThreads ; i++)
	{
		DWORD tmp;
		HANDLE hRet = 
			CreateThread(
			0,
			0,
			renderThread,
			0,
			0,
			&tmp
			);

		if (hRet)
		{
			m_ThreadHandles.push_back(hRet);
			m_ThreadIds.push_back(tmp);
		}
	}
}












