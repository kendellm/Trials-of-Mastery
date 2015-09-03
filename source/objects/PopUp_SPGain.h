#pragma once
#include "popup.h"
class CPopUp_SPGain : public CPopUp
{
public:
	CPopUp_SPGain(void);
	virtual ~CPopUp_SPGain(void);

	virtual void Update	(float fElaspedTime);
	virtual void Render (void)				;

};
