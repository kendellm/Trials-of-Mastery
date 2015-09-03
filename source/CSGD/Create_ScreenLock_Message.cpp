#include "Create_ScreenLock_Message.h"

CCreate_ScreenLock_Message::CCreate_ScreenLock_Message(std::vector<std::vector<int>> waves) : CMessage( MSG_CREATE_SCREEN_LOCK )
{
	Waves = waves;
}
CCreate_ScreenLock_Message::~CCreate_ScreenLock_Message(void)
{

}



