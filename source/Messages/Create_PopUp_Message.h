#pragma once

#include "../CSGD/Message.h"

class Create_PopUp_Message : public CMessage
{

public:
	Create_PopUp_Message(int type = 0);
	~Create_PopUp_Message(void);

	int Type( void ) const { return m_nType;}

private:
	int			m_nType;

};

