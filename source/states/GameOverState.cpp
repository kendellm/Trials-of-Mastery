#include "GameOverState.h"


#include "../CSGD/CSGD_Direct3D.h"
#include "../CSGD/CSGD_TextureManager.h"
#include "../CSGD/CSGD_XAudio2.h"
#include "../CSGD/CSGD_EventSystem.h"

#include "../Inputs/AudioManager.h"
#include "../Inputs/Controller.h"

#include "../Game.h"
#include "StateManager.h"
#include "../states/HomeMenuState.h"
#include "../states/CreditsState.h"
#include "../states/LoadState.h"
#include "../states/OptionState.h"
#include "../states/GameplayState.h"


#include "../GameInfo.h"
#include "../../resource.h"
#include <sstream>

CGameOverState* CGameOverState::GetInstance( void )
{
	static CGameOverState s_Instance;
	return &s_Instance;
}


CGameOverState::CGameOverState(void)
{
	m_nSelect		= -1;
	m_nCursorX		= -1;
	m_nCursorY		= -1;
	m_nCursorImg	= -1;
	m_nLoseBGid     = -1;
	m_nWindBGid		= -1;

	m_bWin			= false;
}


CGameOverState::~CGameOverState(void)
{	
}

void CGameOverState::Enter( void )
{
	

	Active(true);

	m_nSelect = 0;
	m_nCursorX = 20;
	m_nCursorY = 375;

	m_nCursorImg = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/menus/Cursor.png"));


	m_nWindBGid = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/menus/win.png"));


	m_nLoseBGid = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/menus/lose.png"));

	CAudioManager::GetInstance()->Play(BGM::MAIN);
}

void CGameOverState::Exit( void )
{
	m_bWin			= false;

	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nLoseBGid);
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nWindBGid);
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nCursorImg);


	m_nCursorImg = -1;
	m_nLoseBGid     = -1;
	m_nWindBGid		= -1;

	Active(false);
}

bool CGameOverState::Input( void )
{
	CController* pCtrl = CController::GetInstance();
	CAudioManager* pAUM = CAudioManager::GetInstance();

	if( pCtrl->Cancel() )
	{
		pAUM->Play(SFX::CURSOR);
		
		CGame::GetInstance()->GetState()->ChangeState( CHomeMenuState::GetInstance() );
		return true;
	}
		


	if (pCtrl->Confirm())
	{
		pAUM->Play(SFX::CURSOR);
		switch(m_nSelect)
		{
		case EXIT:	   CGame::GetInstance()->GetState()->ChangeState( CHomeMenuState::GetInstance() );
			return true;
			break;
		}
	}

	return true;
}

void CGameOverState::Update( float fElaspedTime )
{
	FontManager* pFont = CGame::GetInstance()->GetFont();
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();


	switch(m_nSelect)
	{
	case EXIT:
		{
			m_nCursorX = 15;
			m_nCursorY = 510;
		}
		break;
	}
}

void CGameOverState::Render( void )
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
	FontManager* pFont = CGame::GetInstance()->GetFont();

	pD3D->Clear(WHITE);

	if(m_bWin)
	{
		if(m_nLevel_Finished == 4)
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nWindBGid, 0, 0);
			pFont->Draw( ARNOVA , FINAL_LEVEL_WIN, 55, 250,
				1.3f, 1.3f, WHITE);
		}
		else
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nWindBGid, 0, 0);
			pFont->Draw( ARNOVA , GAME_WIN, 55, 250,
				1.3f, 1.3f, WHITE);
		}
	}
	else
	{
		CSGD_TextureManager::GetInstance()->Draw(m_nLoseBGid, 0,0);
		pFont->Draw( ARNOVA , GAME_LOSE, 240, 100,
			1.5f,1.5f, WHITE);
	}

	pFont->Draw( ARNOVA , STATE_EXIT, 55, 500);


	CSGD_TextureManager::GetInstance()->Draw(m_nCursorImg, m_nCursorX, m_nCursorY);
}
