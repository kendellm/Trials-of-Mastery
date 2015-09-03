#pragma once
#include "popup.h"
class CPopUp_Fireball_special : public CPopUp
{
	int			m_nBackgroundImgid;
public:
	CPopUp_Fireball_special(void);
	virtual ~CPopUp_Fireball_special(void);

	virtual void Update	(float fElaspedTime);
	virtual void Render (void)				;

};
