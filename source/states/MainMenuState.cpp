/*************************************************************
	File:			MainMenuState.cpp
	Programmer(s):	Kendell Monrose (kmonrose@fullsail.edu)
	Purpose:		Renders and handle input for menu. Serves are root 
					state for menu navigation
*************************************************************/

#include "MainMenuState.h"

#include "../CSGD/CSGD_Direct3D.h"
#include "../CSGD/CSGD_DirectInput.h"
#include "../CSGD/CSGD_TextureManager.h"
#include "../CSGD/CSGD_XAudio2.h"
#include "../CSGD/CSGD_MessageSystem.h"
#include "../Inputs/Controller.h"
#include "../Inputs/AudioManager.h"

#include "../Messages/Create_PopUp_Message.h"

#include "../Game.h"
#include "StateManager.h"
#include "HomeMenuState.h"
#include "CreditsState.h"
#include "LoadState.h"
#include "OptionState.h"
#include "../Util/PopUpManager.h"


#include "../GameInfo.h"
#include "../../resource.h"
#include <sstream>

CMainMenuState* CMainMenuState::GetInstance( void )
{
	static CMainMenuState s_Instance;
	return &s_Instance;
}


CMainMenuState::CMainMenuState(void)
{

	m_nSelect			= -1;
	m_nCursorX			= -1;
	m_nCursorY			= -1;
	m_bFresh			= true;
	m_nMenuArtid		= -1;
	
	m_pGi			= nullptr;
}


CMainMenuState::~CMainMenuState(void)
{	
}

void CMainMenuState::Enter( void )
{
	Active(true);
	m_pAudio = CAudioManager::GetInstance();
	m_pGi = CGame::GetInstance()->GetProfile();

	m_nSelect = 0;
	m_nCursorX = 20;
	m_nCursorY = 375;

	m_nMenuArtid = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/menus/BG_main.png"));


	m_pAudio->Play(BGM::MAIN);
}

void CMainMenuState::Exit( void )
{

	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nMenuArtid);
	

	m_pGi = nullptr;
	m_pAudio = nullptr;
	Active(false);
}

bool CMainMenuState::Input( void )
{

	CController* pCtrl = CController::GetInstance();

	if( pCtrl->Escape() )
	{
		m_nSelect = 4;
	}


	if (pCtrl->CursorMoveUp() )
	{
		if(m_nSelect >0)
			m_nSelect -=1;
		m_pAudio->Play(SFX::CURSOR);
	}

	if (pCtrl->CursorMoveDown() )
	{
		if(m_nSelect <4)
			m_nSelect +=1;
		m_pAudio->Play(SFX::CURSOR);
	}

	if (pCtrl->Confirm())
	{
		switch(m_nSelect)
		{
		case NEWGAME:  CGame::GetInstance()->GetState()->ChangeState( CHomeMenuState::GetInstance() );
			m_bFresh = false;
			return true;
			break;
		case LOADGAME: CGame::GetInstance()->GetState()->ChangeState( CLoadState::GetInstance() );
			return true;
			break;
		case OPTIONS:  CGame::GetInstance()->GetState()->AddState( COptionState::GetInstance());
			return true;
			break;
		case CREDITS:  CGame::GetInstance()->GetState()->ChangeState( CCreditsState::GetInstance() );
			return true;
			break;
		case EXIT: return false;
			break;
		}
		m_pAudio->Play(SFX::CURSOR);
	}

	return true;
}

void CMainMenuState::Update( float fElaspedTime )
{

	switch(m_nSelect)
	{
	case NEWGAME:
		{
			m_nCursorX = 15;
			m_nCursorY = 310;
		}
		break;
	case LOADGAME:
		{
			m_nCursorX = 15;
			m_nCursorY = 385;
		}
		break;
	case OPTIONS:
		{
			m_nCursorX = 15;
			m_nCursorY = 460;
		}
		break;
	case CREDITS:
		{
			m_nCursorX = 15;
			m_nCursorY = 535;
		}
		break;
	case EXIT:
		{
			m_nCursorX = 310;
			m_nCursorY = 560;
		}
		break;
	}
}

void CMainMenuState::Render( void )
{
	
	FontManager* pFont = CGame::GetInstance()->GetFont();
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();

	RECT rScroll = { 40, 621, 257, 686 };
	RECT rCursor = { 292, 643, 308, 662 };

	CSGD_Direct3D::GetInstance()->Clear(WHITE);
	pTM->Draw(m_nMenuArtid, 0, 0);


	pTM->Draw(m_nMenuArtid, 40, 290, 1.0f, 1.0f, &rScroll);
	if(m_bFresh)
	{
		if(CGame::GetInstance()->GetProfile()->Language() == LANG_NEUTRAL)
		pFont->Draw( CHINESE_TAKEAWAY , MM_STATE_NEWGAME,   69, 300, 0.7f, 0.7f, BLACK);
		else
			pFont->Draw( CHINESE_TAKEAWAY , MM_STATE_NEWGAME,   69, 300, 0.6f, 0.6f, BLACK);
	}
	else
	pFont->Draw( CHINESE_TAKEAWAY , MM_STATE_CONTINUE,  69, 300, 0.7f, 0.7f, BLACK);

	pTM->Draw(m_nMenuArtid, 40, 365, 1.0f, 1.0f, &rScroll);
	pFont->Draw( CHINESE_TAKEAWAY , MM_STATE_LOADGAME,  69, 375, 0.7f, 0.7f, BLACK);

	pTM->Draw(m_nMenuArtid, 40, 440, 1.0f, 1.0f, &rScroll);
	pFont->Draw( CHINESE_TAKEAWAY , MM_STATE_OPTIONS,   69, 450, 0.7f, 0.7f, BLACK);
	
	pTM->Draw(m_nMenuArtid, 40, 515, 1.0f, 1.0f, &rScroll);
	pFont->Draw( CHINESE_TAKEAWAY , MM_STATE_CREDITS,   69, 525, 0.7f, 0.7f, BLACK);

	pFont->Draw( ARIAL , STATE_EXIT,	 350, 550, 0.7f, 0.7f, WHITE);


	if(m_nSelect == 4)
	{
		pTM->Draw(m_nMenuArtid, m_nCursorX + 15, m_nCursorY, 1.0f, 1.0f, &rCursor);
		pTM->Draw(m_nMenuArtid, m_nCursorX + 95, m_nCursorY, -1.0f, 1.0f, &rCursor);
	}
	else
	{
		pTM->Draw(m_nMenuArtid, m_nCursorX, m_nCursorY, 1.0f, 1.0f, &rCursor);
		pTM->Draw(m_nMenuArtid, m_nCursorX + 270, m_nCursorY, -1.0f, 1.0f, &rCursor);
	}

}

