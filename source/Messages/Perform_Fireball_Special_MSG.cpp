#include "Perform_Fireball_Special_MSG.h"


CPerform_Fireball_Special_MSG::CPerform_Fireball_Special_MSG(float x, float y, int level, int owner,  bool flip):
	CMessage(MSG_PERFORM_FIREBALL)
{
	m_nPosX = x;
	m_nPosY = y;
	m_nLevel = level;
	m_nOwner = owner;
	m_bFlipped = flip;
}


CPerform_Fireball_Special_MSG::~CPerform_Fireball_Special_MSG(void)
{
}
