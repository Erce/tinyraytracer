/*
*
*	Author : Umut Riza ERTURK
*	March 2010
*	
*
*	This file is the initialize the window
*	and ray tracer
*/

#pragma once

// exceptions are disabled for release mode 
// so disable the warnings 
#pragma warning(disable:4530)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "RayTracer.h"
#include "Sphere.h"
#include "Plane.h"
#include "PointLight.h"


//----------------------------------------
//these are the parameters to be used for
// ray tracer initialization
#define SCREEN_WIDTH	800
#define SCREEN_HEIGHT	600
#define AMBIENT_COLOUR	COLOUR_BLACK
#define NUM_OF_MAX_REFLECTIONS	8
#define SQRT_OF_RAYS_PER_PIXEL	1
#define NUM_OF_THREADS_PER_CPU	1
#define NUM_OF_PIXELS_TO_BE_RENDERED_BY_A_THREAD_AT_A_TIME	100


//----------------------------------------
// Globals for window init
static WNDCLASS		g_WindowClass;
static HWND			g_hWindowClass;
static char			g_pBitmapBuffer[sizeof( BITMAPINFO ) + 16];
static BITMAPINFO*	g_pBitmapInfo;
HDC					g_WindowHdc;


//----------------------------------------
// Globals for ray tracer
Pixel*				g_pScreenPixels = 0;
PixelBuffer*		g_pScreenBuffer = 0;
RayTracer*			g_pRayTracer = 0;
Scene*				g_pScene = 0;
Camera*				g_pCamera = 0;

bool				g_bProcessRunning = true;

void paintWindow();

/*
*	Windows API window callback function
*/
static LRESULT CALLBACK windowCallbackProcedure( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	int result = 0, keycode = 0;
	switch (message)
	{
	case WM_PAINT:
		if (!g_pScreenPixels) break;
		StretchDIBits( g_WindowHdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, g_pScreenPixels, g_pBitmapInfo, DIB_RGB_COLORS, SRCCOPY );
		ValidateRect( g_hWindowClass, NULL );
		break;
	case WM_KEYDOWN:
		if ((wParam & 0xFF) != 27) break;
	case WM_CLOSE:
		ReleaseDC( g_hWindowClass, g_WindowHdc );
		DestroyWindow( g_hWindowClass );
		SystemParametersInfo( SPI_SETSCREENSAVEACTIVE, 1, 0, 0 );
		g_bProcessRunning = false;
		return 0;
		break;
	default:
		result = DefWindowProc(hWnd,message,wParam,lParam);
	}
	return result;
}


/*
*	Paints the window with the current pixel values
*/
void paintWindow()
{
	MSG message;
	HACCEL haccel = 0;
	InvalidateRect( g_hWindowClass,NULL,TRUE );
	SendMessage( g_hWindowClass, WM_PAINT, 0, 0 );
	while (PeekMessage( &message, g_hWindowClass, 0, 0, PM_REMOVE ))
	{
		if (TranslateAccelerator( g_hWindowClass, haccel, &message ) == 0)
		{
			TranslateMessage( &message );
			DispatchMessage( &message );
		}
	}
}




/*
*	inits window
*/
bool prepareWindow()
{
	RECT rectangle;
	g_WindowClass.style			= CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	g_WindowClass.lpfnWndProc	= windowCallbackProcedure;
	g_WindowClass.cbClsExtra	= g_WindowClass.cbWndExtra = 0;
	g_WindowClass.hInstance		= 0;
	g_WindowClass.hIcon			= NULL;
	g_WindowClass.hCursor		= LoadCursor(0,IDC_ARROW);
	g_WindowClass.hbrBackground = NULL;
	g_WindowClass.lpszMenuName	= NULL;
	g_WindowClass.lpszClassName = "tiny raytracer";
	if (!RegisterClass(&g_WindowClass))
	{
		return false;
	}
	rectangle.left				= 0;
	rectangle.top				= 0;
	rectangle.right				= SCREEN_WIDTH;
	rectangle.bottom			= SCREEN_HEIGHT;

	AdjustWindowRect( &rectangle, WS_POPUP|WS_SYSMENU|WS_CAPTION, 0 );

	rectangle.right -= rectangle.left;
	rectangle.bottom -= rectangle.top;
	g_hWindowClass = CreateWindowEx( 0, "tiny raytracer", "tiny raytracer", WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & WS_THICKFRAME,
		CW_USEDEFAULT, CW_USEDEFAULT, rectangle.right, rectangle.bottom, 0, 0, 0, 0 );
	ShowWindow(g_hWindowClass,SW_NORMAL);
	memset(g_pBitmapBuffer,0,sizeof( BITMAPINFOHEADER ) + 16);

	g_pBitmapInfo = (BITMAPINFO *)&g_pBitmapBuffer;
	g_pBitmapInfo->bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
	g_pBitmapInfo->bmiHeader.biPlanes = 1;
	g_pBitmapInfo->bmiHeader.biBitCount = 32;
	g_pBitmapInfo->bmiHeader.biCompression = BI_BITFIELDS;
	g_pBitmapInfo->bmiHeader.biWidth = SCREEN_WIDTH;
	g_pBitmapInfo->bmiHeader.biHeight = -SCREEN_HEIGHT;
	((unsigned long*)g_pBitmapInfo->bmiColors)[0] = 255 << 16;
	((unsigned long*)g_pBitmapInfo->bmiColors)[1] = 255 << 8;
	((unsigned long*)g_pBitmapInfo->bmiColors)[2] = 255;
	g_WindowHdc = GetDC(g_hWindowClass);
	SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, 0, 0, 0);
	return true;
}




/*
*	Inits ray tracer
*/
void initRayTracer()
{
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	// number of threads to be used for ray tracing
	DWORD dwNumOfThreads = sysInfo.dwNumberOfProcessors * NUM_OF_THREADS_PER_CPU;

	// if something got wrong,
	// create one thread
	if(dwNumOfThreads < 1)
	{
		dwNumOfThreads = 0;
	}


	g_pCamera		= new Camera(Vector3(-4,3,0),Vector3(-4,-3,0),Vector3(0,0,1),5);
	g_pScene		= new Scene();
	g_pScreenBuffer = new PixelBuffer(
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		NUM_OF_PIXELS_TO_BE_RENDERED_BY_A_THREAD_AT_A_TIME
		);
	g_pScreenPixels = g_pScreenBuffer->m_pScreenBuffer;
	g_pRayTracer	= new RayTracer(
		dwNumOfThreads,
		g_pScreenBuffer,
		g_pScene,
		g_pCamera,
		AMBIENT_COLOUR, 
		NUM_OF_MAX_REFLECTIONS ,
		SQRT_OF_RAYS_PER_PIXEL
		);
}




/*
*	inits the scene
*/
void initScene()
{
	Material* mat1 = new Material(1.f,.3f,COLOUR_GREEN);
	Material* mat2 = new Material(.5f,.5f,COLOUR_RED);
	Material* mat3 = new Material(.9f,.7f,COLOUR_GREY);
	Material* mat4 = new Material(.01f,.01f,COLOUR_BLUE);
	Material* mat5 = new Material(.9f,.01f,COLOUR_WHITE);


	g_pScene->addVisualObject(new Sphere(Vector3(-60.f, 15.f, 150.f),3.f,mat2));
	g_pScene->addVisualObject(new Sphere(Vector3(-20, 15.f, 150.f),5.f,mat2));
	g_pScene->addVisualObject(new Sphere(Vector3(80.f, 45.f, 250.f),45.f,mat2));
	g_pScene->addVisualObject(new Sphere(Vector3(20.f, 15.f, 150.f),10.f,mat4));
	g_pScene->addVisualObject(new Sphere(Vector3(-100.f, 15.f, 150.f),10.f,mat5));
	g_pScene->addVisualObject(new Sphere(Vector3(-90.f, 95.f, 500.f),115.f,mat3));
	g_pScene->addVisualObject(new Sphere(Vector3(-200.f, 155.f, 350.f),25.f,mat3));

	g_pScene->addVisualObject(new Plane(Vector3::VECTOR_UNIT_Y,25.f,mat1));
	g_pScene->addLightObject(new PointLight(COLOUR_WHITE, Vector3(-1000.f,1000.f,0),1000000.f));
	g_pScene->addLightObject(new PointLight(COLOUR_WHITE, Vector3(500.f,500.f,-500),1000000.f));
}

/*
*	starts ray tracing and loops continuesly
*/
void startRayTracing()
{
	g_pRayTracer->renderScene();

	while (g_bProcessRunning)
	{
		paintWindow();
		Sleep(200); // window painting in every 200 ms
	}
}


/*
*	WIN API main function
*/
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{

	if(prepareWindow())
	{

		initRayTracer();

		initScene();

		startRayTracing();

		SAFE_DELETE(g_pRayTracer);

		CloseHandle(g_hWindowClass);

	}
	else
	{
		ExitProcess( -1 );
		return -1;
	}

	ExitProcess( 0 );
	return 0;
}













