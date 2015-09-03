
#include "HomeMenuState.h"

#include "../CSGD/CSGD_Direct3D.h"
#include "../CSGD/CSGD_TextureManager.h"
#include "../CSGD/CSGD_XAudio2.h"
#include "../CSGD/CSGD_MessageSystem.h"
#include "../CSGD/CSGD_EventSystem.h"
#include "../Messages/CreateMenuPopUp.h"

#include "../Util/PopUpManager.h"
#include "../Inputs/Controller.h"
#include "../Inputs/AudioManager.h"

#include "../CSGD/CSGD_EventSystem.h"


#include "../Game.h"
#include "StateManager.h"
#include "../states/GameplayState.h"
#include "TutorialState.h"
#include "../states/SaveState.h"
#include "../states/MainMenuState.h"
#include "../states/ComingSoonState.h"
#include "../states/ShopState.h"

#include "../GameInfo.h"
#include "../../resource.h"
#include <sstream>


CHomeMenuState* CHomeMenuState::GetInstance( void )
{
	static CHomeMenuState s_Instance;
	return &s_Instance;
}


CHomeMenuState::CHomeMenuState(void)
{
  	m_nSelect		    = -1;	
	m_nCursorX		    = -1;	
	m_nCursorY		    = -1;	
	m_nMenuArtid		= -1;
	m_nCursorKim		= -1;
	m_nLvlImg[0]        = -1;
	m_nLvlImg[1]        = -1;
	m_nLvlImg[2]        = -1;
	m_nLvlImg[3]        = -1;
	m_nLvlImg[4]        = -1;
	
	CSGD_EventSystem::GetInstance()->RegisterClient( "Start_Shop_Tutorial", this );
}


CHomeMenuState::~CHomeMenuState(void)
{
  
}

void CHomeMenuState::Enter( void )
{
	Active(true);

	m_pGi = CGame::GetInstance()->GetProfile();
	CMainMenuState::GetInstance()->FirstRun(false);
	m_pAudio = CAudioManager::GetInstance();

	m_nSelect = 0;

	m_nLvlImg[0]        = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/menus/portrait_tutorial.png"));
	m_nLvlImg[1]        = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/menus/portrait_mountain.png"));
	m_nLvlImg[2]        = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/menus/portrait_plains.png"));
	m_nLvlImg[3]        = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/menus/portrait_forest.png"));
	m_nLvlImg[4]        = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/menus/portrait_desert.jpg"));

	m_nMenuArtid = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/menus/BG_home.png"));

	m_nCursorKim = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/menus/cursor_kim.png"));

	m_pAudio->Play(BGM::HOME);


}

void CHomeMenuState::Exit( void )
{
	if(CGame::GetInstance()->Tutorial() == true)
	{
		CSGD_EventSystem::GetInstance()->SendEventNow("Pop_Up_Completed", (void*)0, (void*)0, (void*)0);
		CSGD_EventSystem::GetInstance()->SendEventNow("First_Time", (void*)0, (void*)0, (void*)0);
	}

	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nCursorKim);
	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nMenuArtid);

	for(int i = 4; i >-1; --i)
	{
		CSGD_TextureManager::GetInstance()->UnloadTexture(m_nLvlImg[i]);
		m_nLvlImg[i]     = -1;
	}
	
	m_nMenuArtid		 = -1;
	m_nCursorKim		 = -1;
	m_pGi			     = nullptr;
	m_pAudio			 = nullptr;
	Active(false);
}

bool CHomeMenuState::Input( void )
{
	CController* pCtrl = CController::GetInstance();

	if(!CGame::GetInstance()->Tutorial() == true)
	{


		if( pCtrl->Cancel() )
		{
			m_pAudio->Play(SFX::CURSOR);
			CGame::GetInstance()->GetState()->ChangeState(CMainMenuState::GetInstance());
			return true;
		}

		if ( pCtrl->CursorMoveUp() )
		{
			if(m_nSelect >0 && m_nSelect < 2)
				m_nSelect -=1;

			if(m_nSelect >= 2  && m_nSelect < CGame::GetInstance()->GetProfile()->Stage() + 3)
				m_nSelect +=1;

			m_pAudio->Play(SFX::CURSOR);

		}

		if (pCtrl->CursorMoveDown() )
		{
			if(m_nSelect <3 )
				m_nSelect +=1;
			else if (m_nSelect >= 3  && m_nSelect <= CGame::GetInstance()->GetProfile()->Stage() + 3)
			{
				m_nSelect -=1;
			}
			m_pAudio->Play(SFX::CURSOR);

		}


		if (pCtrl->CursorMoveRight() )
		{
			if(m_nSelect <3)
				m_nSelect = 3;
			else
			{
				if(m_nSelect <7 && m_nSelect <= CGame::GetInstance()->GetProfile()->Stage() + 2)
					m_nSelect +=1;
			}
			m_pAudio->Play(SFX::CURSOR);

		}

		if (pCtrl->CursorMoveLeft() )
		{
			if(m_nSelect >1)
			{
				m_nSelect -=1;
			}

			m_pAudio->Play(SFX::CURSOR);
		}

	}

	if ( pCtrl->Confirm() )
	{
		if(m_nSelect <= CGame::GetInstance()->GetProfile()->Stage() + 3)
		{
			switch(m_nSelect)
			{	
			case SHOP:		 CGame::GetInstance()->GetState()->ChangeState( CShopState::GetInstance() );	
				return true;
				break;
			case SAVE:		 CGame::GetInstance()->GetState()->ChangeState( CSaveState::GetInstance() );
				return true;
				break;
			case EXIT:		 CGame::GetInstance()->GetState()->ChangeState( CMainMenuState::GetInstance() );
				return true;
				break;
			case TUTORIAL: 
				{
					CGameplayState::GetInstance()->Level(0);
					CGame::GetInstance()->GetState()->LoadState( CGameplayState::GetInstance() );
					return true;
					break;
				}
			case LEVEL1:
				{
					CGameplayState::GetInstance()->Level(1);
					CGame::GetInstance()->GetState()->LoadState( CGameplayState::GetInstance() );
					return true;
					break;
				}
			case LEVEL2:
				{
					CGameplayState::GetInstance()->Level(2);
					CGame::GetInstance()->GetState()->LoadState( CGameplayState::GetInstance() );
					return true;
					break;
				}
			case LEVEL3:
				{
					CGameplayState::GetInstance()->Level(3);
					CGame::GetInstance()->GetState()->LoadState( CGameplayState::GetInstance() );
					return true;
					break;
				}
			case LEVEL4:
				{
					CGameplayState::GetInstance()->Level(4);
					CGame::GetInstance()->GetState()->LoadState( CGameplayState::GetInstance() );
					return true;
					break;
				}
			}
			m_pAudio->Play(SFX::CURSOR);
		}
		else
		{

		}
	}

	return true;
}

void CHomeMenuState::Update( float fElaspedTime )
{
	FontManager* pFont = CGame::GetInstance()->GetFont();
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();


	switch(m_nSelect)
	{
	case SHOP:
		{
			m_nCursorX = 15;
			m_nCursorY = 385;
		}
		break;
	case SAVE:
		{
			m_nCursorX = 15;
			m_nCursorY = 460;
		}
		break;
	case TUTORIAL:
		{
			m_nCursorX = 421;
			m_nCursorY = 450;
		}
		break;
	case LEVEL1:
		{
			m_nCursorX = 460;
			m_nCursorY = 347;
		}
		break;
	case LEVEL2:
		{
			m_nCursorX = 532;
			m_nCursorY = 384;
		}
		break;
	case LEVEL3:
		{
			m_nCursorX = 592;
			m_nCursorY = 295;
		}
		break;
	case LEVEL4:
		{
			m_nCursorX = 674;
			m_nCursorY = 187;
		}
		break;
	case EXIT:
		{
			m_nCursorX = 280;
			m_nCursorY = 575;
		}
		break;
	}
}

void CHomeMenuState::Render( void )
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
	FontManager* pFont = CGame::GetInstance()->GetFont();
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
	RECT rLevel[5] = {
					   {439, 613, 480, 650}, 
					   {483, 612, 523, 653}, 
					   {524, 612, 564, 653}, 
					   {566, 612, 605, 651},
					   {610, 611, 649, 652}
			         };
	int xPos[5] = {439,478,550,610,692 };
	int yPos[5] = {490,387,424,335,227 };

	RECT rScroll = { 40, 621, 257, 686 };
	RECT rCursor = { 292, 643, 308, 662 };

	pD3D->Clear(WHITE);
	pTM->Draw(m_nMenuArtid, 0, 0);



	for(int i = m_pGi->Stage(); i > -1; --i )
	{
		pTM->Draw( m_nMenuArtid, xPos[i],yPos[i],
			1.0f,1.0f,&rLevel[i]);
	}
	

	if(m_nSelect < 3)
	{

		m_pGi->Display(18,57);

		if(m_nSelect == 2)
		{
			pTM->Draw(m_nMenuArtid, m_nCursorX + 15, m_nCursorY, 1.0f, 1.0f, &rCursor);
			pTM->Draw(m_nMenuArtid, m_nCursorX + 185, m_nCursorY, -1.0f, 1.0f, &rCursor);
		}
		else
		{
			pTM->Draw(m_nMenuArtid, m_nCursorX, m_nCursorY, 1.0f, 1.0f, &rCursor);
			pTM->Draw(m_nMenuArtid, m_nCursorX + 270, m_nCursorY, -1.0f, 1.0f, &rCursor);
		}
	}
	else
	{
		RECT box = {14, 18, 339, 226};
		pD3D->DrawHollowRect(box, BROWN );

		switch (m_nSelect-3)
		{
		case 0:  pFont->Draw( ARIAL , HM_STATE_LV0 , 115, 235 );
		break;
		case 1:  pFont->Draw( ARIAL , HM_STATE_LV1 , 115, 235 );
		break;
		case 2:  pFont->Draw( ARIAL , HM_STATE_LV2 , 115, 235 );
		break;
		case 3:  pFont->Draw( ARIAL , HM_STATE_LV3 , 115, 235 );
		break;
		case 4:  pFont->Draw( ARIAL , HM_STATE_LV4 , 115, 235 );
		break;
		}

		RECT rbox = {0, 0, 325, 209};
		pTM->Draw(m_nLvlImg[(m_nSelect-3)],14,18,
					1.0f,1.0f,&rbox);

		RECT rCursor = {0, 0, 74, 61};
		pTM->Draw(m_nCursorKim, m_nCursorX, m_nCursorY,
			1.0f, 1.0f, &rCursor);
	}



	pTM->Draw(m_nMenuArtid, 40, 365, 1.0f, 1.0f, &rScroll);
	pFont->Draw( CHINESE_TAKEAWAY , HM_STATE_SHOP,  70, 375, 0.7f, 0.7f, BLACK);

	pTM->Draw(m_nMenuArtid, 40, 440, 1.0f, 1.0f, &rScroll);
	pFont->Draw( CHINESE_TAKEAWAY , HM_STATE_SAVE,   70, 450, 0.7f, 0.7f, BLACK);

	pFont->Draw( ARIAL  , HM_STATE_MM  , 320, 575,0.8f,0.8f);

	CPopUpManager::GetInstance()->Render();
}

void CHomeMenuState::HandleEvent( CEvent* pEvent )
{
	if( pEvent->GetEventID() == "Start_Shop_Tutorial" )
	{
		
	}
}
