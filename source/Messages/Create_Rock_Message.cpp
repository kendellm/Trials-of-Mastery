#include "Create_Rock_Message.h"


CCreate_Rock_Message::CCreate_Rock_Message(int owner, bool flipped, float x, float y) : CMessage( MSG_ROCK_SPECIAL )
{
	m_nOwner = owner;
	m_bFlipped = flipped;
	m_nPosX = x;
	m_nPosY = y;
}


CCreate_Rock_Message::~CCreate_Rock_Message(void)
{
}
