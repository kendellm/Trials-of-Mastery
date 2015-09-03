#pragma once
#include "../CSGD/message.h"
class CPerform_SubZero_Special_MSG : public CMessage
{
public:
	CPerform_SubZero_Special_MSG(float x, float y, int level, int owner);
	~CPerform_SubZero_Special_MSG(void);


	int   GetOwner  ( void )     { return m_nOwner  ;}
	float GetPosY   ( void )	 { return m_nPosY   ;}
	float GetPosX	( void )	 { return m_nPosX   ;}
	int   GetLevel  ( void )     { return m_nLevel  ;}

	//void  SetOwner  (int actor)	 { m_nOwner = actor ;}
	//void  SetPosX	(float x)	 { m_nPosX  = x     ;}
	//void  SetPosY	(float y)    { m_nPosY  = y     ;}
												  

private:

	int		m_nLevel;
	int		m_nOwner;
	float   m_nPosX;
	float   m_nPosY;

	
};

