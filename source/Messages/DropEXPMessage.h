#pragma once
#include "../CSGD/message.h"

class CDropEXPMessage : public CMessage
{
	int m_nExpPts;
public:
	CDropEXPMessage(int nExp);
	~CDropEXPMessage(void);

	int GetExp()	{return m_nExpPts;}
};

