#include "Create_Rock_Fall_Message.h"

CCreate_Rock_Fall_Message::CCreate_Rock_Fall_Message(int owner, bool flipped) : CCreate_Projectile_Message(owner, flipped)
{
	CMessage::SetMessageID(MSG_CREATE_ROCK_FALL);
}


CCreate_Rock_Fall_Message::~CCreate_Rock_Fall_Message(void)
{
}