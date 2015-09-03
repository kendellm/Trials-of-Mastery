#pragma once
#include "../CSGD/Message.h"

class CPlayer;
class CreatePlayerMessage : public CMessage
{
public:
	CreatePlayerMessage(CPlayer* pPlayer);
	~CreatePlayerMessage(void);

	CPlayer*  GetPlayer() {return m_pPlayer;}

private:
	CPlayer*	m_pPlayer;

};

