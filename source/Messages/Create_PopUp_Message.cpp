#include "Create_PopUp_Message.h"

Create_PopUp_Message::Create_PopUp_Message(int type) : CMessage(MSG_CREATE_POPUP)
{
	m_nType = type;
}


Create_PopUp_Message::~Create_PopUp_Message(void)
{
}
