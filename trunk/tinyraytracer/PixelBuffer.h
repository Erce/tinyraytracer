/*
*
*	Author : Umut Riza ERTURK
*	March 2010
*	
*
*	This is the pixel buffer class which
*	enables ray tracer to run in multithread
*	by protecting the pixelbuffer with mutex
*/

#include "Colour3f.h"

class PixelBuffer
{
public:
	void*			m_hPixelBufferMutex;
	Pixel*			m_pScreenBuffer;
	Pixel*			m_pCursorPosition;
	unsigned int	m_uiCurrentX;
	unsigned int	m_uiCurrentY;

	unsigned int	m_uiBufferChunkSizeToBeRendered;

	unsigned int	m_uiWidth;
	unsigned int	m_uiHeight;

	unsigned int	m_uiFrameCount;

	PixelBuffer(unsigned int uiWidth, unsigned int uiHeight, unsigned int uiBufferChunkSize = 100);
	~PixelBuffer();

	__forceinline void PixelBuffer::getNextPixels(Pixel* &pBuffer, unsigned int& uiStartX, unsigned int& uiStartY)
	{
		// lock the critical resource so that the other 
		// thread wont violate concurrency
		WaitForSingleObject(m_hPixelBufferMutex,INFINITE);

		// set the resource start point to be given
		// to the client thread
		pBuffer = m_pCursorPosition;

		// update the cursor position for the next request
		m_pCursorPosition = m_pCursorPosition + m_uiBufferChunkSizeToBeRendered;
		uiStartX = m_uiCurrentX;
		uiStartY = m_uiCurrentY;

		// forward X
		m_uiCurrentX = m_uiCurrentX + m_uiBufferChunkSizeToBeRendered;
		
		// if reaches the end of the line
		if (m_uiCurrentX == m_uiWidth) // since cannot be bigger
		{
			// go down
			m_uiCurrentY++;
			// reset from the new line
			m_uiCurrentX = 0;

			// reaches the end of the screen
			// increase the frame count and reset
			if(m_uiCurrentY == m_uiHeight)
			{
				m_uiCurrentY = 0;
				m_pCursorPosition = m_pScreenBuffer;
				m_uiFrameCount++;
			}
		}

		// unlock the critical resource
		ReleaseMutex(m_hPixelBufferMutex);
	}
};
