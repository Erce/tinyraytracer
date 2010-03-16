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
	void getNextPixels(Pixel* &pBuffer, unsigned int& uiStartX, unsigned int& uiStartY);

};
