#pragma once
#include "../CSGD/Message.h"

class CCreateMenuPopUp :	public CMessage
{

public:
	CCreateMenuPopUp(int type = 0);
	~CCreateMenuPopUp(void);

	int Type( void ) const { return m_nType;}

private:
	int			m_nType;

};
