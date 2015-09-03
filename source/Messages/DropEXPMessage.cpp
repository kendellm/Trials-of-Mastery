#include "DropEXPMessage.h"



CDropEXPMessage::CDropEXPMessage( int nExp ): CMessage(MSG_DROP_EXP)
{
	m_nExpPts = nExp;
}


CDropEXPMessage::~CDropEXPMessage(void)
{
}
