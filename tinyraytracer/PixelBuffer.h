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

	PixelBuffer(unsigned int uiWidth, unsigned int uiHeight, unsigned int uiBufferChunkSizeToBeRendered);
	~PixelBuffer();
	void getNextPixels(Pixel* &pBuffer, unsigned int& uiStartX, unsigned int& uiStartY);

};
