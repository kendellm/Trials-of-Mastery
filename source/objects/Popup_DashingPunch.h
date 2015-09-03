#pragma once
#include "PopUp.h"

class CPopUp_DashingPunch : public CPopUp
{
public:
	CPopUp_DashingPunch(void);
	virtual ~CPopUp_DashingPunch(void);

	virtual void Update	(float fElaspedTime);
	virtual void Render (void)				;

private:

	int keys[3];
};

