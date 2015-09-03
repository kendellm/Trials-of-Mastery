#include "CreateMenuPopUp.h"


CCreateMenuPopUp::CCreateMenuPopUp(int type ): CMessage(MSG_CREATE_MENU_POPUP)
{
	m_nType = type;

}


CCreateMenuPopUp::~CCreateMenuPopUp(void)
{
}
