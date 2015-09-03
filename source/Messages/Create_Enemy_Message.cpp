#include "Create_Enemy_Message.h"
#include "../objects/Player.h"
#include "../CSGD/Message.h"


Create_Enemy_Message::Create_Enemy_Message(int type, CPlayer* target) 
	: CMessage( MSG_CREATE_ENEMY )
{
	Enemy_Type = type;
	Target = target;
}
Create_Enemy_Message::~Create_Enemy_Message(void)
{
	Enemy_Type = 0;
}


