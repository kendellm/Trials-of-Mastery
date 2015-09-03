/*
File:		Create_Iceball_Message.cpp
Author:		Joseph Trefethen (j.trefethen@fullsail.edu)
Course:		SGP 1307
Purpose:	CCreate_IceBall_Message class creates an Iceball proejctile
for the Iceboss
*/

#include "Create_IceBall_Message.h"
#include "../objects/Human.h"


CCreate_IceBall_Message::CCreate_IceBall_Message(float fPosX, float fPoxY, CHuman* pOwner, bool bFlipped) 
	: CMessage(MSG_ICEBALL)
{
	m_nPosX			= fPosX;
	m_nPosY			= fPoxY;
	m_nOwner		= pOwner;
	m_bFlipped		= bFlipped;
}



CCreate_IceBall_Message::~CCreate_IceBall_Message()
{

}

