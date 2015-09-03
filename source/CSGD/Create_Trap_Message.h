#pragma once

#include "Message.h"

class Create_Trap_Message : public CMessage
{
private:
	int Trap_Type;
	float PosX;
	float PosY;

public:
	Create_Trap_Message(int type = 0, float x = 0.0f, float y = 0.0f);
	~Create_Trap_Message(void);

	int GetTrapType() { return Trap_Type;}
	float GetPosX() { return PosX;}
	float GetPosY() { return PosY;}

	void SetTrapType(int t) { Trap_Type = t;}
	void SetPosX(float x) { PosX = x;}
	void SetPosY(float y) {PosY = y;}



};

