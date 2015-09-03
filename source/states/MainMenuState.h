/*************************************************************
	File:			MainMenuState.h
	Programmer(s):	Kendell Monrose (kmonrose@fullsail.edu)
	Purpose:		Renders and handle input for menu. Serves are root 
					state for menu navigation
*************************************************************/
#pragma once

#include "IGamestate.h"

class GameInfo;
class CAudioManager;
class CMainMenuState : public IGameState
{
public:
	enum CURSOR {NEWGAME, LOADGAME, OPTIONS, CREDITS, EXIT};

	static CMainMenuState* GetInstance( void );

	virtual	void Update	(float fElaspedTime) override;
	virtual	void Render (void)				 override;
	virtual bool Input	(void)				 override;
	virtual void Enter	(void)				 override;
	virtual void Exit	(void)				 override;

			void FirstRun(bool b) {m_bFresh = b;}

private:

	CMainMenuState						( void );
	CMainMenuState						( const CMainMenuState& );
	virtual         ~CMainMenuState		( void );
	CMainMenuState& operator=			( const CMainMenuState& );

	int			m_nSelect;
	int			m_nCursorX;
	int			m_nCursorY;
	bool		m_bFresh;

	int			m_nMenuArtid;


	GameInfo*		m_pGi;
	CAudioManager*  m_pAudio;

};

