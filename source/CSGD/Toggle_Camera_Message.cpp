#include "Toggle_Camera_Message.h"

CToggle_Camera_Message::CToggle_Camera_Message(bool set_To) : CMessage(MSG_TOGGLE_CAMERA_LOCK)
{
	Set_To = set_To;
}

CToggle_Camera_Message::~CToggle_Camera_Message(void)
{
}
