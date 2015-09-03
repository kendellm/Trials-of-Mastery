/*************************************************************
	File:			IGameState.h
	Programmer(s):	Kendell Monrose (kmonrose@fullsail.edu)
	Purpose:		Base interface for all game states
*************************************************************/

#pragma once
class IGameState
{
public:


	virtual ~IGameState(void)=0{}

	bool		 Active ( void )	    {return m_bActive;}
	void		 Active ( bool _active) {m_bActive = _active;}


	virtual	void Update	(float fElaspedTime)  = 0;
	virtual	void Render (void)				  = 0;
	virtual bool Input	(void)				  = 0;
	virtual void Enter	(void)				  = 0;
	virtual void Exit	(void)				  = 0;

private:

	// not being initialized, could cause release mode bug
	bool		m_bActive;

};

