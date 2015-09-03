#pragma once
#include "../CSGD/Message.h"

class CCreate_Wind_Tornado_Message : public CMessage
{
private:
	int m_nOwner;
	float m_nPosX;
	float m_nPosY;
	bool m_bFlipped;

public:
	CCreate_Wind_Tornado_Message(int owner = 0, bool flipped = true, float x = 0, float y = 0);
	~CCreate_Wind_Tornado_Message(void);

	int GetOwner() { return m_nOwner;}
	void SetOwner(int i) {m_nOwner = i;}

	bool GetFlipped(){return m_bFlipped;}
	void SetFlipped(bool b){m_bFlipped = b;}

	float GetPosX(){return m_nPosX;}
	void SetPosX(float x){m_nPosX = x;}

	float GetPosY(){return m_nPosY;}
	void SetPosY(float y){m_nPosY = y;}
};

