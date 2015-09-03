/*************************************************************
	File:			CreditsState.h
	Programmer(s):	Kendell Monrose (kmonrose@fullsail.edu)
*************************************************************/
#pragma once
#include "IGamestate.h"
class CCreditsState : public IGameState
{
public:

	static CCreditsState* GetInstance( void );

	virtual	void Update	(float fElaspedTime) override;
	virtual	void Render (void)				 override;
	virtual bool Input	(void)				 override;
	virtual void Enter	(void)				 override;
	virtual void Exit	(void)				 override;

private:

	CCreditsState						( void );
	CCreditsState						( const CCreditsState& );
	virtual         ~CCreditsState		( void );
	CCreditsState& operator=			( const CCreditsState& );

	int			m_nBackgroundImgID;

};
