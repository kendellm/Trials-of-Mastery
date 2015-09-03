#pragma once
#include "Message.h"
class CToggle_Camera_Message :	public CMessage
{
private:
	bool Set_To;

public:
	CToggle_Camera_Message(bool set_To = false);
	~CToggle_Camera_Message(void);

	bool GetSet_To() { return Set_To;}
	void SetSet_To(bool b) {Set_To = b;}
};

