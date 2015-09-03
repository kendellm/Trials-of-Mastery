#include "ShopState.h"

#include "../Game.h"
#include "../GameInfo.h"
#include "../../resource.h"
#include "StateManager.h"
#include "HomeMenuState.h"

#include "../CSGD/CSGD_MessageSystem.h"
#include "../CSGD/CSGD_EventSystem.h"
#include "../Messages/CreateMenuPopUp.h"
#include "../Util/PopUpManager.h"
#include "TutorialState.h"

#include "../CSGD/CSGD_Direct3D.h"
#include "../CSGD/CSGD_TextureManager.h"
#include "../CSGD/CSGD_XAudio2.h"
#include "../CSGD/SGD_String.h"
#include "../CSGD/SGD_Util.h"
#include "../Inputs/Controller.h"
#include "../Inputs/AudioManager.h"
#include "../Util/ItemManager.h"


CShopState* CShopState::GetInstance( void )
{
	static CShopState s_Instance;
	return &s_Instance;
}


CShopState::CShopState(void)
{
		m_nSelect	     = 0;	
		m_nSubSelect	 = 0;
		m_nCursorX		 = 0;
		m_nCursorY		 = 0;
		m_nSubPage		 = 0;
		m_nScrollImgid =-1;
		m_nMenuArtid = -1;

		m_fBuy			 = -1;
		m_pIM			 = nullptr;




}


CShopState::~CShopState(void)
{
		
}

void CShopState::Enter( void )
{
	Active(true);
	m_pIM = new CItemManager;
	m_pIM->Initialize("config/shop.xml", CGame::GetInstance()->GetProfile());

	m_nSelect = 0;
	m_nSubPage = 0;
	m_nSubSelect = 0;
	m_fBuy		= -1;
	m_nCursorX = 5;
	m_nCursorY = 37;

	m_nMenuArtid = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/menus/BG_shop.png"));

	m_nScrollImgid = CSGD_TextureManager::GetInstance()->LoadTexture(
		_T("resource/graphics/menus/scroll_L.png"));

}

void CShopState::Exit( void )
{

	if(CGame::GetInstance()->Tutorial() == true)
	{
		CSGD_EventSystem::GetInstance()->SendEventNow("Pop_Up_Completed", (void*)0, (void*)0, (void*)0);
		CGame::GetInstance()->Tutorial(false);
		
	}

	CSGD_TextureManager::GetInstance()->UnloadTexture(m_nMenuArtid);

	m_pIM->Release();
	SAFE_DELETE(m_pIM);

	m_nMenuArtid =-1;

	CSGD_EventSystem::GetInstance()->UnregisterClientAll( this );

	Active(false);
}

bool CShopState::Input( void )
{
	CController* pCtrl = CController::GetInstance();
	CAudioManager* pAUM = CAudioManager::GetInstance();

	if(m_nSubPage == 0)
	{
		if( pCtrl->Cancel() )
		{
			pAUM->Play(SFX::CURSOR);

			CGame::GetInstance()->GetState()->ChangeState( CHomeMenuState::GetInstance() );
			return true;
		}

		if ( pCtrl->CursorMoveUp() )
		{	
			/*
			if(m_nSelect >0)
				m_nSelect = max( m_nSelect -2, -1);
			else 
				m_nSelect = max( m_nSelect - 1, 0);
				*/

			m_nSelect = max( m_nSelect -1, -1);
				if(m_nSelect < 0)
					m_nSelect = 7;
			pAUM->Play(SFX::CURSOR);

		}


		if ( pCtrl->CursorMoveDown() )
		{
			/*if(m_nSelect <5 && m_nSelect > 0)
				m_nSelect = min( m_nSelect +2, 6);
			else */


			m_nSelect = min( m_nSelect + 1, 8);
			if(m_nSelect > 7)
				m_nSelect = 0;

			pAUM->Play(SFX::CURSOR);

		}



		if (pCtrl->Confirm() )
		{

			if(m_nSelect == 7)
			{
				pAUM->Play(SFX::CURSOR);

				CGame::GetInstance()->GetState()->ChangeState( CHomeMenuState::GetInstance() );
				return true;
			}
			else
			{
				m_nSubPage = m_nSelect+1;	

				if( m_pIM->PriceCheck(m_nSelect) )
				{
					m_nSelect = 1;
					pAUM->Play(SFX::CURSOR);

				}
				else
				{
					m_fBuy = 1;
					m_nSubPage = 0;	
					pAUM->Play(SFX::DENIED);
				}
			}
		}
	}
	else
	{
		if(pCtrl->Cancel() )
		{
			m_nSelect = m_nSubPage-1;
			m_nSubPage = 0;
			pAUM->Play(SFX::CURSOR);
		}

		if ( pCtrl->CursorMoveUp() || pCtrl->CursorMoveLeft() )
		{
			if(m_nSelect >0)
				m_nSelect -=1;
			pAUM->Play(SFX::CURSOR);
		}

		if ( pCtrl->CursorMoveDown() || pCtrl->CursorMoveRight()  )
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
				m_nSelect = m_nSubPage-1;
				m_nSubPage = 0;

			}
			else
			{
				if(m_pIM->PriceCheck(m_nSubPage-1) == true)
				{
					m_pIM->Buy(m_nSubPage-1);
					m_fBuy = 0;
				}

				m_nSelect = m_nSubPage-1;
				m_nSubPage = 0;

			}
		}
	}

	return true;
}

void CShopState::Update( float fElaspedTime )
{
	if(m_nSubPage == 0)
	{

		switch(m_nSelect)
		{
		case HEALTH:
			{
					m_nCursorX = 10;
					m_nCursorY = 55;
			}
			break;
		case SPECIAL:
			{
					 m_nCursorX = 10;
					 m_nCursorY = 130;
			}
			break;
		case LIVES:
			{
					m_nCursorX = 10;
					m_nCursorY = 205;
			}
			break;
		case FIRE:
			{
					m_nCursorX = 10;
					m_nCursorY = 280;
			}
			break;
		case WIND:
			{
					m_nCursorX = 10;
					m_nCursorY = 355;
			}
			break;
		case ICE:
			{
					m_nCursorX = 10;
					m_nCursorY = 430;
			}
			break;
		case EARTH:
			{
					m_nCursorX = 10;
					m_nCursorY = 505;
		}
		break;
		case EXIT:
			{
				   m_nCursorX = 76;
				   m_nCursorY = 560;
			}
		}
	}
	else
	{
		if(m_nSelect == 0)
		{
			m_nCursorX = 415;
			m_nCursorY = 161;
		}
		else
		{
			m_nCursorX = 556;
			m_nCursorY = 161;
		}
	}

	if(m_fBuy > 0)
		m_fBuy = max(m_fBuy - fElaspedTime, -1);

}

void CShopState::Render( void )
{
	CSGD_Direct3D* pD3D = CSGD_Direct3D::GetInstance();
	FontManager* pFont = CGame::GetInstance()->GetFont();
	CSGD_TextureManager* pTM = CSGD_TextureManager::GetInstance();
	TOSTRINGSTREAM wss;

	pD3D->Clear(WHITE);
	pTM->Draw(m_nMenuArtid, 0, 0);
	RECT rScroll = { 40, 621, 257, 686 };
	RECT rCursor = { 292, 643, 308, 662 };

	RECT rIcon[4] = {
		{575, 617, 654, 692}, 
		{498, 690, 575, 770}, 
		{576, 692, 654, 770}, 
		{502, 617, 576, 692}
	};
	int xPos[5] = {439,478,550,610,692 };
	int yPos[5] = {490,387,424,335,227 };

	pTM->Draw(m_nMenuArtid, 27,  31, 1.0f, 1.0f, &rScroll);
	pTM->Draw(m_nMenuArtid, 27, 106, 1.0f, 1.0f, &rScroll);
	pTM->Draw(m_nMenuArtid, 27, 181, 1.0f, 1.0f, &rScroll); 
	pTM->Draw(m_nMenuArtid, 27, 256, 1.0f, 1.0f, &rScroll);
	pTM->Draw(m_nMenuArtid, 27, 331, 1.0f, 1.0f, &rScroll);
	pTM->Draw(m_nMenuArtid, 27, 406, 1.0f, 1.0f, &rScroll);
	pTM->Draw(m_nMenuArtid, 27, 481, 1.0f, 1.0f, &rScroll);

	pFont->Draw(CHINESE_TAKEAWAY, PLAYER_HEALTH	,	 95,   45, 0.7f, 0.7f, BLACK );
	pFont->Draw(CHINESE_TAKEAWAY, PLAYER_SPECIAL,	 95,  120, 0.7f, 0.7f, BLACK );
	pFont->Draw(CHINESE_TAKEAWAY, PLAYER_LIVES	,	115,  195, 0.7f, 0.7f, BLACK );
	pFont->Draw(CHINESE_TAKEAWAY, PLAYER_FIRE	,	115,  270, 0.7f, 0.7f, BLACK );
	pFont->Draw(CHINESE_TAKEAWAY, PLAYER_WIND	,	115,  345, 0.7f, 0.7f, BLACK );
	pFont->Draw(CHINESE_TAKEAWAY, PLAYER_ICE 	,	115,  420, 0.7f, 0.7f, BLACK );
	pFont->Draw(CHINESE_TAKEAWAY, PLAYER_EARTH	,	115,  495, 0.7f, 0.7f, BLACK );

	pTM->Draw(m_nMenuArtid, 29,257, 1.0f,1.0f, &rIcon[0] );
	pTM->Draw(m_nMenuArtid, 29,332, 1.0f,1.0f, &rIcon[1] );
	pTM->Draw(m_nMenuArtid, 29,407, 1.0f,1.0f, &rIcon[2] );
	pTM->Draw(m_nMenuArtid, 29,482, 1.0f,1.0f, &rIcon[3] );

	pFont->Draw( ARIAL , STATE_EXIT, 106, 550, 0.8f,1.0f, WHITE);


		if(m_nSubPage == 0  && m_nSelect >=0 && m_nSelect < 7)
		{
			pTM->Draw( m_nScrollImgid,356, 71 );

			if(m_fBuy > 0)
			{
				pFont->Draw( CHINESE_TAKEAWAY, SHOP_STATE_FAIL, 400, 108, 0.6f,0.6f );
			}
			else
			{
				m_pIM->Item(m_nSelect);
				int alpha = 255;
				if( !CGame::GetInstance()->GetProfile()->CheckGrade(m_nSelect, m_pIM->Level() ) )
				{
					alpha = 100;	
				}

				wss.str(L"");
				wss << m_pIM->Name().c_str() << " Lv" << m_pIM->Level();
				pFont->Draw( CHINESE_TAKEAWAY, wss.str().c_str(),390,108, 0.7f,0.7f ,
					D3DCOLOR_ARGB(alpha, 255, 255, 255) );

				wss.str(L"");
				wss << m_pIM->Price();
				pFont->Draw( CHINESE_TAKEAWAY, wss.str().c_str(),534,154, 0.7f,0.7f,
					D3DCOLOR_ARGB(alpha, 255, 255, 255) );
				pFont->Draw( CHINESE_TAKEAWAY, L"pts",664,154, 0.5f,0.5f,
					D3DCOLOR_ARGB(alpha, 255, 255, 255) );			
			}
				pTM->Draw(m_nMenuArtid, m_nCursorX, m_nCursorY, 1.0f, 1.0f, &rCursor);
				pTM->Draw(m_nMenuArtid, m_nCursorX + 250, m_nCursorY, -1.0f, 1.0f, &rCursor);
		}
		else if(m_nSubPage > 0)
		{
			pTM->Draw( m_nScrollImgid, 356, 71 );

			wss.str(L"");
			wss << "Buy " << m_pIM->Name().c_str() << "? ";
			pFont->Draw( CHINESE_TAKEAWAY, wss.str().c_str(), 400, 108, 0.5f, 0.5f );
			pFont->Draw( CHINESE_TAKEAWAY, STATE_YES, 430, 150, 0.7f, 0.7f  );
			pFont->Draw( CHINESE_TAKEAWAY, STATE_NO,  571, 150, 0.7f, 0.7f  );
	
			pTM->Draw(m_nMenuArtid, m_nCursorX, m_nCursorY, 1.0f, 1.0f, &rCursor);
		}

		if(m_nSelect == 7)
		{
			pTM->Draw(m_nMenuArtid, m_nCursorX, m_nCursorY, 1.0f, 1.0f, &rCursor);
			pTM->Draw(m_nMenuArtid, m_nCursorX + 85, m_nCursorY, -1.0f, 1.0f, &rCursor);
		}
		
		wss.str(L"");
		wss << m_pIM->EXP();
		pFont->Draw( CHINESE_TAKEAWAY, wss.str().c_str(),450,530, 1.0f,1.0f, BLACK );
		pFont->Draw( ARNOVA, L"exp",635,533, 1.0f,1.0f, BLACK);
}


void CShopState::HandleEvent( CEvent* pEvent )
{
	
}
