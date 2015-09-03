#pragma once
#include "popup.h"
class CPopUps_Traps : public CPopUp
{
public:
	CPopUps_Traps(void);
	virtual ~CPopUps_Traps(void);

	virtual void Update	(float fElaspedTime);
	virtual void Render (void)				;

private:

	int m_nBGImgid;
};
