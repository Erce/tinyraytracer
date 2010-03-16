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

	for (int i =0; i < m_uiNumOfThreads; i++)
	{
		TerminateThread(m_ThreadHandles[i],0);
		CloseHandle(m_ThreadHandles[i]);

	}
	SAFE_DELETE(m_pScene);
	SAFE_DELETE(m_pPixelBuffer);
	SAFE_DELETE(m_pCamera);

}

int iX = 0;
void RayTracer::renderPixel(std::stack<Ray> &rays, Colour3f& rColour)
{
	int iDepth=0;
	std::vector<std::pair<Colour3f,VisualObject*>> colours;
	Real rCumulativeReflect=1.f;
	bool bNoIntersection = true;
	while(!rays.empty())
	{
		Colour3f tmpColour(0);
		iDepth++;
		VisualObject *pVisaulObject = 0;
		Real rDistance = MAX_FLOAT;
		Ray rRay = rays.top();
		rays.pop();
		IntersectionType intersectionType = m_pScene->getNearestIntersection(rRay,pVisaulObject, rDistance );
		if (pVisaulObject)
		{	
			bNoIntersection = false;
			Vector3& rRayDir = rRay.dir;
			Vector3 vIntersectionPoint = rRay.point + rRayDir * (rDistance + intersectionType * EPSILON);

			Vector3 vNormalAtIntersection = pVisaulObject->getNormal(vIntersectionPoint);

			const unsigned int uiSz = m_pScene->m_LightObjects.size();
			PointLight *pLight ;
			
			std::vector<PointLight*>& lightObjects = m_pScene->m_LightObjects;
			// shading computation
			for (int k = 0; k < uiSz; k++)
			{
				pLight = lightObjects[k];

					Real rLightSourceDistance;
					Real rLightSourceDistanceSQ;

					Vector3 vDirLight = pLight->m_Position - vIntersectionPoint;
					rLightSourceDistanceSQ = vDirLight.squaredLength();
					rLightSourceDistance = MathUtil::sqrtX(rLightSourceDistanceSQ);
					vDirLight /= rLightSourceDistance;
					Real rBrightnessFactor = 1.f / (rLightSourceDistanceSQ) ;// rLightSourceDistance;
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
							tmpColour += (powf(rDot,20) * pVisaulObject->m_pMaterial->m_rSpecular * rBrightnessFactor) * cLightColour;
						}
					}				
	
				
			}

			rColour += tmpColour * rCumulativeReflect;
			// reflected ray computation
			if(iDepth<m_uiTraceDepth)
			{
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

int g_threadId=1;
DWORD WINAPI renderThread(LPVOID param)
{
	int threadNo = g_threadId++;
	Pixel* pixelsToBeRendered;
	unsigned int uiStartX;
	unsigned int uiStartY;
	Colour3f colourOfPixel = gs_pRayTracer->m_AmbientLight;
	Vector3 &vrTopLeft = gs_pRayTracer->m_pCamera->m_NearPlane.m_TopLeft ;
	Vector3 vStartPoint;
	Vector3 &vrCameraPoint = gs_pRayTracer->m_pCamera->m_CameraPoint;
	unsigned int uiRayPerPixel = gs_pRayTracer->m_uiRayPerPixel;
	Ray ray;
	//int i = 100;
	while(1)
	{
		gs_pRayTracer->m_pPixelBuffer->getNextPixels( pixelsToBeRendered, uiStartX, uiStartY );
		vStartPoint =  vrTopLeft + (gs_pRayTracer->m_vWidthStepSize * (Real)uiStartX * uiRayPerPixel ) + (gs_pRayTracer->m_vHeightStepSize * (Real)uiStartY * uiRayPerPixel) ;
		ray.point = vrCameraPoint;
		for(unsigned int uiPixelIndex = 0 ; uiPixelIndex < gs_pRayTracer->m_uiNumOfPixels; uiPixelIndex++)
		{
			std::stack<Ray> rays;
			colourOfPixel =  Colour3f(0);/*gs_pRayTracer->m_AmbientLight*/;
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
					colourOfPixel += colourOfRender;
					vTmpStartPoint += gs_pRayTracer->m_vWidthStepSize;
				}
				vTmpStartPoint += gs_pRayTracer->m_vHeightStepSize;
			}
			vStartPoint += gs_pRayTracer->m_vWidthStepSize * uiRayPerPixel;
			clampColour3fToPixel(colourOfPixel * gs_pRayTracer->m_rDivFactor, *pixelsToBeRendered);
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












