#pragma once
#include "Create_Projectile_Message.h"

class CCreate_Rock_Fall_Message : public CCreate_Projectile_Message
{
public:
	CCreate_Rock_Fall_Message(int owner = 0, bool flipped = true);
	~CCreate_Rock_Fall_Message(void);
};

