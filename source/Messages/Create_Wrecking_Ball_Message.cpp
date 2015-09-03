#include "Create_Wrecking_Ball_Message.h"


CCreate_Wrecking_Ball_Message::CCreate_Wrecking_Ball_Message(int owner, bool flipped) : CCreate_Projectile_Message(owner, flipped)
{
	CMessage::SetMessageID(MSG_CREATE_WRECKING_BALL);
}


CCreate_Wrecking_Ball_Message::~CCreate_Wrecking_Ball_Message(void)
{
}
