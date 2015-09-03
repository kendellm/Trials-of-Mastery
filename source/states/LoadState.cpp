#include "LoadState.h"


#include "../CSGD/CSGD_Direct3D.h"
#include "../CSGD/CSGD_TextureManager.h"
#include "../CSGD/CSGD_XAudio2.h"


#include "../Game.h"
#include "StateManager.h"
#include "../states/MainMenuState.h"
#include "../states/HomeMenuState.h"
#include "../Inputs/Controller.h"
#include "../Inputs/AudioManager.h"

#include "../GameInfo.h"
#include "../../resource.h"
#include <sstream>


CLoadState* CLoadState::GetInstance( void )
{
	static CLoadState s_Instance;
	return &s_Instance;
}


CLoadState::CLoadState(void)
{
	m_pGi = nullptr;
	m_nSelect		= -1;	
	m_nCursorX		= -1;
	m_nCursorY		= -1;
	m_nMenuArtid= -1;
	
}


CLoadState::~CLoadState(void)
{

}

void CLoadState::Enter( void )
{
	Active(true);

	m_pGi = CGame::GetInstance()->GetProfile();

	RefreshSlots();
	m_nSelect = 0;
	m_nCursorX = 20;
	m_nCursorY = 375;

	m_nMenuArtid = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/menus/BG_blank.png"));


	m_nScrollImgid = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/menus/scroll_XL.png"));

}

void CLoadState::Exit( void )
{
	for(unsigned int i = 0; i < m_vSlots.size(); ++i)
	{
		if(m_vSlots[i] != nullptr)
			delete m_vSlots[i];
	}
	m_vSlots.clear();

	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nMenuArtid);
	m_nMenuArtid=-1;
	m_pGi = nullptr;
}

bool CLoadState::Input( void )
{
	CController* pCtrl = CController::GetInstance();
	CAudioManager* pAUM = CAudioManager::GetInstance();

	if (pCtrl->CursorMoveUp() )
	{
		if(m_nSelect >0)
			m_nSelect -=1;
		pAUM->Play(SFX::CURSOR);
	}

	if (pCtrl->CursorMoveDown() )
	{
		if(m_nSelect <3)
			m_nSelect +=1;
		pAUM->Play(SFX::CURSOR);

	}

	if ( pCtrl->Confirm())
	{
		switch(m_nSelect)
		{
		case SLOT1: Load(1);
			break;
		case SLOT2: Load(2);
			break;
		case SLOT3: Load(3);
			break;
		case EXIT:  CGame::GetInstance()->GetState()->ChangeState( CMainMenuState::GetInstance() );
			return true;
			break;
		}
		
		pAUM->Play(SFX::CURSOR);
	}

	return true;
}

void CLoadState::Update( float fElaspedTime )
{
	FontManager* pFont = CGame::GetInstance()->GetFont();
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();


	switch(m_nSelect)
	{
	case SLOT1:
		{
			m_nCursorX = 25;
			m_nCursorY = 360;
		}
		break;
	case SLOT2:
		{
			m_nCursorX = 25;
			m_nCursorY = 435;
		}
		break;
	case SLOT3:
		{
			m_nCursorX = 25;
			m_nCursorY = 510;
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

void CLoadState::Render( void )
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
	FontManager* pFont = CGame::GetInstance()->GetFont();
	RECT rScroll = { 40, 621, 257, 686 };
	RECT rCursor = { 292, 643, 308, 662 };

	pD3D->Clear(WHITE);
	CSGD_TextureManager::GetInstance()->Draw(m_nMenuArtid,0,0);

	
	if(m_nSelect < 3)
	{

		if(m_vSlots[m_nSelect] == nullptr)
		{
			CSGD_TextureManager::GetInstance()->Draw(m_nScrollImgid, 18, 57, 1.0f, 1.0f);

			pFont->Draw( CHINESE_TAKEAWAY , SAV_STATE_EMPTY, 137, 116,
			0.7f,0.7f, BLACK);	
		}
		else
			m_vSlots[m_nSelect]->Display(18,57);	
	}
	


	CSGD_TextureManager::GetInstance()->Draw(m_nMenuArtid, 45, 340, 1.0f, 1.0f, &rScroll);
	pFont->Draw( CHINESE_TAKEAWAY , SAV_STATE_SLOT1, 75, 350, 0.7f, 0.7f, BLACK);

	CSGD_TextureManager::GetInstance()->Draw(m_nMenuArtid, 45, 415, 1.0f, 1.0f, &rScroll);
	pFont->Draw( CHINESE_TAKEAWAY , SAV_STATE_SLOT2, 75, 425, 0.7f, 0.7f, BLACK);
	
	CSGD_TextureManager::GetInstance()->Draw(m_nMenuArtid, 45, 490, 1.0f, 1.0f, &rScroll);
	pFont->Draw( CHINESE_TAKEAWAY , SAV_STATE_SLOT3, 75, 500, 0.7f, 0.7f, BLACK);

	pFont->Draw( ARIAL , STATE_EXIT, 350, 550,
		0.8f,1.0f, D3DCOLOR_ARGB(255,255, 255, 255));


	if(m_nSelect == 3)
	{
		CSGD_TextureManager::GetInstance()->Draw(m_nMenuArtid, m_nCursorX + 15, m_nCursorY, 1.0f, 1.0f, &rCursor);
		CSGD_TextureManager::GetInstance()->Draw(m_nMenuArtid, m_nCursorX + 95, m_nCursorY, -1.0f, 1.0f, &rCursor);
	}
	else
	{
		CSGD_TextureManager::GetInstance()->Draw(m_nMenuArtid, m_nCursorX, m_nCursorY, 1.0f, 1.0f, &rCursor);
		CSGD_TextureManager::GetInstance()->Draw(m_nMenuArtid, m_nCursorX + 260, m_nCursorY, -1.0f, 1.0f, &rCursor);
	}

}

void CLoadState::Load( int _slot )
{
	m_pGi = CGame::GetInstance()->LoadProfile(_slot);
	if(m_pGi != nullptr)
	{
		CGame::GetInstance()->FirstRun(false);
		CGame::GetInstance()->Import(m_pGi);
		CGame::GetInstance()->GetState()->ChangeState(CHomeMenuState::GetInstance());
	}
	else
	{
		CAudioManager::GetInstance()->Play(SFX::DENIED);
		m_nSelect = 3;
		
	}
}

void CLoadState::RefreshSlots( void )
{
	for(unsigned int i = 0; i < m_vSlots.size(); ++i)
	{
		if(m_vSlots[i] != nullptr)
		delete m_vSlots[i];
		m_vSlots[i] = nullptr;
	}

	m_vSlots.clear();

	for(int i = 0; i < 3; ++i)
	{
		m_vSlots.push_back(CGame::GetInstance()->LoadProfile(i+1));
	}
}

