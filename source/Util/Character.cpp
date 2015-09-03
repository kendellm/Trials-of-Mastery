#include "Character.h"


CCharacter::CCharacter(void)
{

	 m_nPosX		= 0;
	 m_nPosY		= 0;
	 m_nWidth		= 0;
	 m_nHeight		= 0;
	 m_nXOffset		= 0;
	 m_nYOffset		= 0;
	 m_nXAdvance	= 0;
}

CCharacter::CCharacter( int x, int y, int w, int h,
							int xOffset, int yOffset, int Xadvance )
{
	m_nPosX			= x;
	m_nPosY			= y;
	m_nWidth		= w;
	m_nHeight		= h;
	m_nXOffset		= xOffset;
	m_nYOffset		= yOffset;
	m_nXAdvance		= Xadvance;
}


CCharacter::~CCharacter(void)
{
}
