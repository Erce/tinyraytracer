/*
*
*	Author : Umut Riza ERTURK
*	March 2010
*	
*/

#include "PixelBuffer.h"


/************************************************************************/
/* init the pixel buffer                                               */
/************************************************************************/
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

	// number of pixels to be rendered by a thread at a time
	// should be an exact divider of the width in order to fasten up
	while(uiWidth % uiBufferChunkSize != 0)
	{
		uiBufferChunkSize++;
	}

	m_uiBufferChunkSizeToBeRendered = uiBufferChunkSize;
	m_pScreenBuffer = new Pixel[uiWidth * uiHeight];
	memset( m_pScreenBuffer, 0, sizeof(Pixel)*uiHeight*uiWidth );
	m_pCursorPosition = m_pScreenBuffer;
	
	// create a mutex for concurrency
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

