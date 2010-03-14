#include <windows.h>
#include "RayTracer.h"
#include "Sphere.h"
#include <vector>
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
	delete m_pPixelBuffer;
}

int iX = 0;
void RayTracer::renderPixel(std::stack<Ray> &rays, Colour3f& rColour)
{
	int iDepth=0;
	std::vector<std::pair<Colour3f,VisualObject*>> colours;
	Real rCumulativeReflect=1.f;
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
			if (pVisaulObject->m_bIsLightEmiter)
			{
				rColour=Colour3f(1.0f);
			}
			else
			{

			

				Vector3& rRayDir = rRay.dir;
				Vector3 vIntersectionPoint = rRay.point + rRayDir * (rDistance + intersectionType * EPSILON);

				Vector3 vNormalAtIntersection = pVisaulObject->getNormal(vIntersectionPoint);

				const unsigned int uiSz = m_pScene->m_uiNumLightObjects;
				Sphere *pSphereLigt = 0;
				VisualObject *pLight = 0;
				

				// shading computation
				for (int k = 0; k < uiSz; k++)
				{
					pLight = m_pScene->m_pLightObjects[k];
					if(pLight->m_Type == VOT_SPHERE)
					{
						Real rLightSourceDistance;
						pSphereLigt = (Sphere*) pLight;
						Vector3 vDirLight = pSphereLigt->m_Origin - vIntersectionPoint;
						rLightSourceDistance = vDirLight.length();
						vDirLight /= rLightSourceDistance;
						Real rBrightnessFactor = 1.f ;// rLightSourceDistance;
						if( m_pScene->anyNonLightIntersects(Ray(vDirLight,vIntersectionPoint),rLightSourceDistance) == false)
						{

							Real rNormalDotLightDir = vNormalAtIntersection.dot(vDirLight);
							//  diffuse lighting
							Colour3f &cLightColour = pSphereLigt->m_Material.m_RColour;
							Colour3f &cVisaulObjectColour = pVisaulObject->m_Material.m_RColour;
							if(rNormalDotLightDir > 0)
							{
								tmpColour += (rBrightnessFactor * rNormalDotLightDir * pVisaulObject->m_Material.m_rDiffuse)
									* (cVisaulObjectColour * cLightColour);

							}

							// specular lighting
							Vector3 vTmp = vDirLight - ( (2.f * rNormalDotLightDir) * vNormalAtIntersection);
							Real rDot = vTmp.dot(rRayDir);
							if (rDot > 0)
							{
								tmpColour += (powf(rDot,20) * pVisaulObject->m_Material.m_RSpecular * rBrightnessFactor) * cLightColour;
							}
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
					rCumulativeReflect *= pVisaulObject->m_Material.m_rReflectivity;
					iDepth++;
				}
			}
		}
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
	int threadNo[]={1,2,3,4,5,6,7,8,9,10};
	for(int i = 0; i < m_uiNumOfThreads ; i++)
	{
	
	CreateThread(
		0,
		0,
		renderThread,
		&(threadNo[i]),
		0,
		&(m_ThreadIds[i])
		);
	}
}












