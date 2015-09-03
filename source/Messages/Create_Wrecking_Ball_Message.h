#pragma once
#include "Create_Projectile_Message.h"

class CCreate_Wrecking_Ball_Message : public CCreate_Projectile_Message
{
public:
	CCreate_Wrecking_Ball_Message(int owner = 0, bool flipped = true);
	~CCreate_Wrecking_Ball_Message(void);
};

