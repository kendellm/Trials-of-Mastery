#include "Create_Wind_Tornado_Message.h"


CCreate_Wind_Tornado_Message::CCreate_Wind_Tornado_Message(int owner, bool flipped, float x, float y) : CMessage( MSG_WIND_TORNADO )
{
	m_nOwner = owner;
	m_bFlipped = flipped;
	m_nPosX = x;
	m_nPosY = y;
}


CCreate_Wind_Tornado_Message::~CCreate_Wind_Tornado_Message(void)
{
}
