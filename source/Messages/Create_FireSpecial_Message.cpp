#include "Create_FireSpecial_Message.h"


CCreate_FireSpecial_Message::CCreate_FireSpecial_Message(int owner, bool flipped, float x, float y) : CMessage( MSG_FIRE_SPECIAL )
{
	m_nOwner = owner;
	m_bFlipped = flipped;
	m_nPosX = x;
	m_nPosY = y;
}


CCreate_FireSpecial_Message::~CCreate_FireSpecial_Message(void)
{
}
