#include "CreatePlayerMessage.h"
#include "../objects/Player.h"

CreatePlayerMessage::CreatePlayerMessage(CPlayer* pPlayer): CMessage(MSG_CREATE_PLAYER)
{
	m_pPlayer = pPlayer;
}


CreatePlayerMessage::~CreatePlayerMessage(void)
{

}
