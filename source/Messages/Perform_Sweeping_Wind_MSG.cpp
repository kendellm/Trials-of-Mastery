#include "Perform_Sweeping_Wind_MSG.h"


CPerform_Sweeping_Wind_MSG::CPerform_Sweeping_Wind_MSG(float x, float y, int level, int owner, bool flip):
	CMessage(MSG_PERFORM_SWEEPING_WIND)
{
	m_nPosX = x;
	m_nPosY = y;
	m_nLevel = level;
	m_nOwner = owner;
	m_bFlipped = flip;
}


CPerform_Sweeping_Wind_MSG::~CPerform_Sweeping_Wind_MSG(void)
{
}
