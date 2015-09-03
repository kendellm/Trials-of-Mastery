#include "Create_FireBoss_Fireball_Message.h"


CCreate_FireBoss_Fireball_Message::CCreate_FireBoss_Fireball_Message(int owner, bool flipped) : CMessage( MSG_FIREBOSS_FIREBALL )
{
	m_nOwner = owner;
	m_bFlipped = flipped;
}


CCreate_FireBoss_Fireball_Message::~CCreate_FireBoss_Fireball_Message(void)
{
}
