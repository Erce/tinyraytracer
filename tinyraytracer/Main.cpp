#pragma once

#include <windows.h>
#include <stdlib.h>
#include <iostream>
#include "RayTracer.h"
#include "Sphere.h"
#include "Plane.h"



#define SCRWIDTH	800
#define SCRHEIGHT	600

static WNDCLASS wc;
static HWND wnd;
static char bitmapbuffer[sizeof( BITMAPINFO ) + 16];
static BITMAPINFO* bh;
HDC window_hdc;
Pixel* pBuffer = 0;
PixelBuffer* pPixelBuffer = 0;
RayTracer *pRayTracer = 0;

void DrawWindow();

static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	int result = 0, keycode = 0;
	switch (message)
	{
	case WM_PAINT:
		if (!pBuffer) break;
		StretchDIBits( window_hdc, 0, 0, SCRWIDTH, SCRHEIGHT, 0, 0, SCRWIDTH, SCRHEIGHT, pBuffer, bh, DIB_RGB_COLORS, SRCCOPY );
		ValidateRect( wnd, NULL );
		break;
	case WM_KEYDOWN:
		if ((wParam & 0xFF) != 27) break;
	case WM_CLOSE:
		ReleaseDC( wnd, window_hdc );
		DestroyWindow( wnd );
		SystemParametersInfo( SPI_SETSCREENSAVEACTIVE, 1, 0, 0 );
		ExitProcess( 0 );
		break;
	default:
		result = DefWindowProc(hWnd,message,wParam,lParam);
	}
	return result;
}

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	RECT rect;
	int cc;
	wc.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.hInstance = 0;
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(0,IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "raytracer";
	if (!RegisterClass(&wc)) return FALSE;
	rect.left = rect.top = 0;
	rect.right = SCRWIDTH;
	rect.bottom = SCRHEIGHT;
	AdjustWindowRect( &rect, WS_POPUP|WS_SYSMENU|WS_CAPTION, 0 );
	rect.right -= rect.left, rect.bottom -= rect.top;
	wnd = CreateWindowEx( 0, "raytracer", "raytracer", WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & WS_THICKFRAME,
		CW_USEDEFAULT, CW_USEDEFAULT, rect.right, rect.bottom, 0, 0, 0, 0 );
	ShowWindow(wnd,SW_NORMAL);
	for ( int cc = 0; cc < sizeof( BITMAPINFOHEADER ) + 16; cc++ )	bitmapbuffer[cc] = 0;
	bh = (BITMAPINFO *)&bitmapbuffer;
	bh->bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
	bh->bmiHeader.biPlanes = 1;
	bh->bmiHeader.biBitCount = 32;
	bh->bmiHeader.biCompression = BI_BITFIELDS;
	bh->bmiHeader.biWidth = SCRWIDTH;
	bh->bmiHeader.biHeight = -SCRHEIGHT;
	((unsigned long*)bh->bmiColors)[0] = 255 << 16;
	((unsigned long*)bh->bmiColors)[1] = 255 << 8;
	((unsigned long*)bh->bmiColors)[2] = 255;
	window_hdc = GetDC(wnd);
	SystemParametersInfo(SPI_SETSCREENSAVEACTIVE, 0, 0, 0);
	// prepare output canvas

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	


	Camera *pCamera = new Camera(Vector3(-4,3,0),Vector3(-4,-3,0),Vector3(0,0,1),10);
	Scene *pScene = new Scene();
	pPixelBuffer = new PixelBuffer(SCRWIDTH,SCRHEIGHT,100);
	pBuffer = pPixelBuffer->m_pScreenBuffer;
	pRayTracer = new RayTracer(sysInfo.dwNumberOfProcessors,pPixelBuffer,pScene,pCamera,Colour3f(.5,.5,.5),4,1);
	Material mat1;
	mat1.m_RColour.x = .80f;
	mat1.m_RColour.y = .60f;
	mat1.m_RColour.z = .60f;
	mat1.m_rDiffuse = .7f;
	mat1.m_rReflectivity = .1f;
	mat1.m_RSpecular = .6f;

	Material mat2;
	mat2.m_RColour.x = .70f;
	mat2.m_RColour.y = .70f;
	mat2.m_RColour.z = .70f;
	mat2.m_rDiffuse = .6f;
	mat2.m_rReflectivity = .8f;
	mat2.m_RSpecular = .4;

	Material mat3;
	mat3.m_RColour.x = .80f;
	mat3.m_RColour.y = .80f;
	mat3.m_RColour.z = .80f;
	mat3.m_rDiffuse = 1.f;
	mat3.m_rReflectivity = 1.f;
	mat3.m_RSpecular = 1.f;

	for (int i = 0; i < 10; i++)
	{
		for ( int j = 0; j < 10; j++)
		{
			for ( int k = 0; k < 1; k++)
			{
				pScene->addVisualObject(new Sphere(Vector3(-40.f + i* 9.3f, -10.f + j* 5.3f, 300 - k* 10),2.5f,mat2));
			}
			mat2.m_RColour.x += j / 255.f;
		}
	}

	pScene->addVisualObject(new Sphere(Vector3(-15, 20, 95),20.0f,mat3));

	pScene->addVisualObject(new Plane(Vector3::UNIT_Y,13.f,mat1));
	pScene->addLightObject(new Sphere(Vector3(0,30.f,35.f),1.1f,mat3));

//	pScene->addLightObject(new Sphere(Vector3(-300,300.f,1000.f),10.1f,mat3));
	
	pRayTracer->renderScene();

	while (1)
	{
		DrawWindow();
		Sleep(100);
	}
	return 1;
}

void DrawWindow()
{
	MSG message;
	HACCEL haccel = 0;
	InvalidateRect( wnd,NULL,TRUE );
	SendMessage( wnd, WM_PAINT, 0, 0 );
	while (PeekMessage( &message, wnd, 0, 0, PM_REMOVE ))
	{
		if (TranslateAccelerator( wnd, haccel, &message ) == 0)
		{
			TranslateMessage( &message );
			DispatchMessage( &message );
		}
	}
	Sleep( 0 );
}














