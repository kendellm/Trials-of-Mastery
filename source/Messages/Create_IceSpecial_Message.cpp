#include "Create_IceSpecial_Message.h"


CCreate_IceSpecial_Message::CCreate_IceSpecial_Message(int owner, bool flipped, float x, float y) : CMessage( MSG_ICE_SPECIAL )
{
	m_nOwner = owner;
	m_bFlipped = flipped;
	m_nPosX = x;
	m_nPosY = y;
}


CCreate_IceSpecial_Message::~CCreate_IceSpecial_Message(void)
{
}
