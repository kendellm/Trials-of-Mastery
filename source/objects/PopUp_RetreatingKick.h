#pragma once
#include "popup.h"
class CPopUp_RetreatingKick : public CPopUp
{
public:
	CPopUp_RetreatingKick(void);
	virtual ~CPopUp_RetreatingKick(void);

	virtual void Update	(float fElaspedTime);
	virtual void Render (void)				;

private:

	int keys[3];
};

