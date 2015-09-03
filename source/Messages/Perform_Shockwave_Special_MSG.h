#pragma once
#include "../CSGD/message.h"

class CPerform_Shockwave_Special_MSG : public CMessage
{
public:
	CPerform_Shockwave_Special_MSG(float x, float y, int level=1, int owner=0, bool flip=true);
	~CPerform_Shockwave_Special_MSG(void);


	int   GetOwner  ( void )     { return m_nOwner  ;}
	float GetPosY   ( void )	 { return m_nPosY   ;}
	float GetPosX	( void )	 { return m_nPosX   ;}
	int   GetLevel  ( void )     { return m_nLevel  ;}
	bool  GetFlipped( void )	 { return m_bFlipped;}
private:

	int		m_nLevel;
	int		m_nOwner;
	float   m_nPosX;
	float   m_nPosY;
	bool	m_bFlipped;


};

