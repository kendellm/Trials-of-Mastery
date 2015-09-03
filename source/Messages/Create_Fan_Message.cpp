#include "Create_Fan_Message.h"


CCreate_Fan_Message::CCreate_Fan_Message(int owner, bool flipped, float x, float y) : CMessage( MSG_WIND_FAN )
{
	m_nOwner = owner;
	m_bFlipped = flipped;
	m_nPosX = x;
	m_nPosY = y;
}


CCreate_Fan_Message::~CCreate_Fan_Message(void)
{
}
