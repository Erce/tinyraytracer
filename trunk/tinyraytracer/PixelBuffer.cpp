#include "PixelBuffer.h"
#include <Windows.h>

PixelBuffer::PixelBuffer(unsigned int uiWidth, unsigned int uiHeight, unsigned int uiBufferChunkSize)
{
	m_uiCurrentX = 0;
	m_uiCurrentY = 0;
	m_uiWidth = uiWidth;
	m_uiHeight = uiHeight;
	m_uiFrameCount = 0;
	if(uiBufferChunkSize == 0)
	{
		uiBufferChunkSize = uiWidth;
	}
	if (uiBufferChunkSize > uiWidth)
	{
		uiBufferChunkSize = uiWidth;
	}
	while(uiWidth % uiBufferChunkSize != 0)
	{
		uiBufferChunkSize++;
	}

	m_uiBufferChunkSizeToBeRendered = uiBufferChunkSize;
	m_pScreenBuffer = new Pixel[uiWidth * uiHeight];
	memset( m_pScreenBuffer, 0, sizeof(Pixel)*uiHeight*uiWidth );
	m_pCursorPosition = m_pScreenBuffer;

	m_hPixelBufferMutex = CreateMutex( 
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex

}
PixelBuffer::~PixelBuffer()
{
	CloseHandle(m_hPixelBufferMutex);
	SAFE_DELETE_ARRAY(m_pScreenBuffer);

}
int test = 0; 
void PixelBuffer::getNextPixels(Pixel* &pBuffer, unsigned int& uiStartX, unsigned int& uiStartY)
{
	WaitForSingleObject(m_hPixelBufferMutex,INFINITE);

	test += m_uiBufferChunkSizeToBeRendered;
	pBuffer = m_pCursorPosition;

	m_pCursorPosition = m_pCursorPosition + m_uiBufferChunkSizeToBeRendered;
	uiStartX = m_uiCurrentX;
	uiStartY = m_uiCurrentY;

	m_uiCurrentX = m_uiCurrentX + m_uiBufferChunkSizeToBeRendered;
	if (m_uiCurrentX == m_uiWidth) // since cannot be bigger
	{
		m_uiCurrentY++;
		m_uiCurrentX = 0;
		if(m_uiCurrentY == m_uiHeight)
		{
			m_uiCurrentY = 0;
			m_pCursorPosition = m_pScreenBuffer;
			m_uiFrameCount++;
		}
	}

	ReleaseMutex(m_hPixelBufferMutex);
}