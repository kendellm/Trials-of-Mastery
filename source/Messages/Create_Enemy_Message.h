#pragma once

#include "../CSGD/Message.h"


class CPlayer;

class Create_Enemy_Message : public CMessage
{
private:
	int Enemy_Type;
	CPlayer* Target;

public:
	Create_Enemy_Message(int type = 0, CPlayer* target = nullptr);
	~Create_Enemy_Message(void);

	int Get_Enemy_Type() { return Enemy_Type;}
	CPlayer* Get_Target() { return Target;}
};

