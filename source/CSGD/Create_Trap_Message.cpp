#include "Create_Trap_Message.h"
#include "Message.h"

Create_Trap_Message::Create_Trap_Message(int type, float x, float y) : CMessage( MSG_CREATE_TRAP )
{
	Trap_Type = type;
	PosX = x;
	PosY = y;
}
Create_Trap_Message::~Create_Trap_Message(void)
{
	Trap_Type = 0;
}