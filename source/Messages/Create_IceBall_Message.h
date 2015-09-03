/*
	File:		Create_Iceball_Message.h
	Author:		Joseph Trefethen (j.trefethen@fullsail.edu)
	Course:		SGP 1307
	Purpose:	CCreate_IceBall_Message class creates an Iceball proejctile
				for the Iceboss
*/

#ifndef CREATE_ICEBALL_MESSAGE_H
#define CREATE_ICEBALL_MESSAGE_H

#include "../CSGD/Message.h"

class CHuman;

class CCreate_IceBall_Message : public CMessage
{
public:
	CCreate_IceBall_Message(float fPosX, float fPoxY, CHuman* pOwner, bool bFlipped);
	~CCreate_IceBall_Message();

	// Accessors
	CHuman*		GetOwner	( void )     { return m_nOwner;		}
	float		GetPosY		( void )	 { return m_nPosY;		}
	float		GetPosX		( void )	 { return m_nPosX;		}
	bool		GetFlipped	( void )	 { return m_bFlipped;	}


private:

	CHuman*		m_nOwner;
	float		m_nPosX;
	float		m_nPosY;
	bool		m_bFlipped;

};











#endif