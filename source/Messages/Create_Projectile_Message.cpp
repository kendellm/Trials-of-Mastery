#include "Create_Projectile_Message.h"


CCreate_Projectile_Message::CCreate_Projectile_Message(int owner, bool flipped) : CMessage( MSG_CREATE_PROJECTILE )
{
	m_nOwner = owner;
	m_bFlipped = flipped;
	m_szAnimationName = "";
}


CCreate_Projectile_Message::CCreate_Projectile_Message(float fPosX, float fPosY, const char * szAnimName, int nOwner, bool bFlipped)
	:CMessage(MSG_CREATE_PROJECTILE)
{
	m_nOwner		= nOwner;
	m_nPosX			= fPosX;
	m_nPosY			= fPosY;
	m_bFlipped		= bFlipped;
	m_szAnimationName = szAnimName;
}



CCreate_Projectile_Message::~CCreate_Projectile_Message(void)
{
}
