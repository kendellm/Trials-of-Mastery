#include "PauseMenuState.h"

#include "../CSGD/CSGD_Direct3D.h"
#include "../CSGD/CSGD_TextureManager.h"
#include "../Inputs/Controller.h"
#include "../Inputs/AudioManager.h"

#include "../Game.h"
#include "StateManager.h"
#include "MainMenuState.h"
#include "OptionState.h"
#include "HomeMenuState.h"
#include "TutorialState.h"
#include "../../resource.h"


CPauseMenuState* CPauseMenuState::GetInstance( void )
{
	static CPauseMenuState s_Instance;
	return &s_Instance;
}


CPauseMenuState::CPauseMenuState(void)
{
	m_nSelect		= -1;

	m_nCursorX		= -1;
	m_nCursorY		= -1;
	m_nSubPage		= -1;
}


CPauseMenuState::~CPauseMenuState(void)
{

}

void CPauseMenuState::Enter( void )
{
	Active(true);

	m_nSelect = 0;
	m_nSubPage = 0;
	m_nCursorX = 20;
	m_nCursorY = 375;

	m_nMenuArtid = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/menus/BG_options.png"));
}

void CPauseMenuState::Exit( void )
{
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nMenuArtid);
	m_nMenuArtid = -1;
}

bool CPauseMenuState::Input( void )
{
	CController* pCtrl = CController::GetInstance();
	CAudioManager*	 pAUM  =  CAudioManager::GetInstance();

	if(m_nSubPage == 0)
	{
		if(pCtrl->Cancel() )
		{
			pAUM->Play(SFX::CURSOR);

			 CGame::GetInstance()->GetState()->RemoveState(this);
		}

		if ( pCtrl->CursorMoveUp() )
		{
			m_nSelect = max(m_nSelect - 1, -1);
			if(m_nSelect == -1)
				m_nSelect = 4;
			pAUM->Play(SFX::CURSOR);
		}


		if ( pCtrl->CursorMoveDown() )
		{
			m_nSelect = min(m_nSelect + 1, 5);
			if(m_nSelect == 5)
				m_nSelect = 0;
			pAUM->Play(SFX::CURSOR);
		}


		if (pCtrl->Confirm() )
		{
			pAUM->Play(SFX::CURSOR);

			switch(m_nSelect)
			{
			case CONTINUE:  CGame::GetInstance()->GetState()->RemoveState(this);
				return true;
				break;
			case COMBO:  
				CTutorialState::GetInstance()->SetInstruction(0);
				CGame::GetInstance()->GetState()->AddState( CTutorialState::GetInstance());
				return true;
				break;
			case OPTIONS:   CGame::GetInstance()->GetState()->AddState(COptionState::GetInstance());
				return true;
				break;
			case CONTROLS:  
				CTutorialState::GetInstance()->SetInstruction(1);
				CGame::GetInstance()->GetState()->AddState( CTutorialState::GetInstance());
				return true;
				break;
			case QUIT:
				{
					m_nSubPage = 1;
					m_nSelect = 1;
				}
				break;
			}
		}

	}
	else
	{
		if(pCtrl->Cancel() )
		{
			m_nSubPage = 0;
			m_nSelect = 2;
			pAUM->Play(SFX::CURSOR);

		}

		if (pCtrl->CursorMoveUp())
		{
			if(m_nSelect >0)
				m_nSelect -=1;
			pAUM->Play(SFX::CURSOR);

		}

		if (pCtrl->CursorMoveDown() )
		{
			if(m_nSelect <1)
				m_nSelect +=1;
			pAUM->Play(SFX::CURSOR);

		}


		if (pCtrl->Confirm() )
		{
			pAUM->Play(SFX::CURSOR);

			if(m_nSelect ==1)
			{
				m_nSubPage = 0;
				m_nSelect = 2;
			}
			else
			{
				//exit to home menu
				CGame::GetInstance()->GetState()->RevertState( CHomeMenuState::GetInstance() );
			}
		}
	}


	return true;
}

void CPauseMenuState::Update( float fElaspedTime )
{
	FontManager* pFont = CGame::GetInstance()->GetFont();
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();

	if(m_nSubPage == 0)
	{
		switch(m_nSelect)
		{
		case CONTINUE:
			{
				m_nCursorX = 235;
				m_nCursorY = 190;
			}
			break;
		case COMBO:
			{
				m_nCursorX = 235;
				m_nCursorY = 218;
			}
			break;
		case OPTIONS:
			{
				m_nCursorX = 235;
				m_nCursorY = 239;
			}
			break;
		case CONTROLS:
			{
				m_nCursorX = 235;
				m_nCursorY = 259;
			}
			break;
		case QUIT:
			{
				m_nCursorX = 235;
				m_nCursorY = 290;
			}
			break;

		}
	}
	else
	{
		if(m_nSelect == 0)
		{
			m_nCursorX = 260;
			m_nCursorY = 210;
		}
		else if(m_nSelect ==1)
		{
			m_nCursorX = 260;
			m_nCursorY = 260;
		}
	}
}

void CPauseMenuState::Render( void )
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
	FontManager* pFont = CGame::GetInstance()->GetFont();
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();

	RECT rBackground = { 206, 128, 637, 425};
	RECT rScroll	 = { 277, 618, 491, 679};
	RECT rCursor     = { 523, 640, 544, 660};


	RECT rDimBG = {0,0, CGame::GetInstance()->ScreenWidth(),
	CGame::GetInstance()->ScreenHeight()};
	pD3D->DrawRect(rDimBG, D3DCOLOR_ARGB(100, 0,0,0));

	pTM->Draw( m_nMenuArtid, 184, 82, 1.0f, 1.0f, &rBackground );


	pTM->Draw(m_nMenuArtid,293, 62, 1.0f, 1.0f, &rScroll );
	pFont->Draw( CHINESE_TAKEAWAY , PAU_STATE_TITLE, 335, 70, 0.8f,0.8f, BLACK);

	if(m_nSubPage == 0)
	{
	
		pFont->Draw( ARIAL , PAU_STATE_CON		,257 , 187,	0.8f,	0.8f,  BROWN);
		pFont->Draw( ARIAL , PAU_STATE_COMBO	,257 , 216,	0.8f,	0.8f,  BROWN);
		pFont->Draw( ARIAL , PAU_STATE_OPS		,257 , 236,	0.8f,	0.8f,  BROWN);
		pFont->Draw( ARIAL , PAU_STATE_CONTROLS	,257 , 256,	0.8f,	0.8f,  BROWN);
		pFont->Draw( ARIAL , PAU_STATE_QIT		,257 , 290,	0.8f,	0.8f,  BROWN);
	}
	else
	{

		pFont->Draw( ARNOVA , STATE_YES, 300, 200,
			0.9f,0.9f, BLACK);

		pFont->Draw( ARNOVA , STATE_NO, 300, 250,
			0.9f,0.9f, BLACK);
	}


	pTM->Draw(m_nMenuArtid, m_nCursorX, m_nCursorY,
		1.0f, 1.0f, &rCursor,0.0f, 0.0f,0.0f);

}